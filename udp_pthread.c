#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h> 
#include <sys/time.h>    // for gettimeofday()
#include <signal.h>
#include <assert.h>
#include <errno.h>
#include <sys/syslog.h>
#include <pthread.h>
#include <fcntl.h> 
#include "mysql_connect.h" //gcc -I/usr/include/mysql/  -L/usr/lib64/mysql/ -lmysqlclient -lz main.c mysql_connect.c -o testudpmain && ./testudpmain
#include "stack_data.h"
#define PORT_SERV 8911

struct thread_arg{
    int num;
    struct datainfo *pdatainfo;
};
struct datainfo mydatainfo;
//格式化时间
void get_timestr(char *filename)
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep);
    sprintf(filename, "%d-%.2d-%.2d-%.2d:%.2d",(1900+p->tm_year), (1+p->tm_mon), p->tm_mday,  p->tm_hour, p->tm_min);
}



void* wdb_fun(void *arg)
{
    struct thread_arg *plist = (struct thread_arg *)arg;
    push(plist->pdatainfo, plist->num);
    struct datainfo *pdata = plist->pdatainfo;
    free(plist);
    plist = NULL;
    return (void*)0;
}

void* th_fun(void *arg)
{
    FILE *fp = NULL;
    fp = fopen("test1.txt", "a+");
    fputs("a\n", fp);                    
    fclose(fp);
	return (void*)0;
}

//处理函数
void* dealdata_fun(void *arg)
{
    struct datainfo *pdata =  (struct datainfo *)arg;
    int flag = 0;
    if(arg == NULL){
    }else{
        flag = pthread_mutex_lock(&pdata->mutex);
        if( flag!= 0){
            debug_log("pthread_mutex_lock error", strerror(flag));        
        }
        int num = pop(pdata);
        if(num >= 0){
            char write[1204]={'\0'};
            char filename[30] = {0};
            get_timestr(filename);
            sprintf(write, "insert into test_udp (key_value, key_time) values(%d, '%s');", num, filename);
            insertsql(write);
            // FILE *fp = NULL;
            // char filename[30] = {0};
            // char buff[1200] = {'\0'};
            // sprintf(buff,"%d\n", num);
            // // printf("a=%s", buff);
            // get_timestr(filename);
            // fp = fopen(filename, "a+");
            // fputs(buff, fp);                    
            // fclose(fp);
        }
        flag = pthread_mutex_unlock(&pdata->mutex);
        if( flag!= 0){
            debug_log("pthread_mutex_unlock error", strerror(flag));        
        }
    }
	return (void*)0;
}

//循环处理
void* dealwhile_fun(void *arg)
{
    int tot = 0;
    int flag = 0;
    while(1){
        flag = pthread_mutex_lock(&mydatainfo.mutex);
        if(mydatainfo.top != 0){
            tot ++; 
            int num = pop(&mydatainfo);
            char write[1204]={'\0'};
            char filename[30] = {0};
            get_timestr(filename);
            sprintf(write, "insert into test_udp (key_value, key_time) values(%d, '%s');", num, filename);
            insertsql(write);
        }
        flag = pthread_mutex_unlock(&mydatainfo.mutex);
    }
    return (void*)0;
} 


//设置非阻塞  
static void setnonblocking(int sockfd) {  
    int flag = fcntl(sockfd, F_GETFL, 0);  
    if (flag < 0) {  
        perror("fcntl F_GETFL fail");  
        return;  
    }  
    if (fcntl(sockfd, F_SETFL, flag | O_NONBLOCK) < 0) {  
        perror("fcntl F_SETFL fail");  
    }  
}  

static int handle_connect(int s)
{
    struct sockaddr_in addr_clie;
    socklen_t len;
    int n;
    pid_t pid;  
    pthread_attr_t attr;
    int err;
    int flag;
    
    flag = init(&mydatainfo);
    if(flag != 1){
        debug_log("create datainfo error", "null");
        return -1;
    }
    // setnonblocking(s);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_t th_deal;
    if((err = pthread_create(&th_deal, &attr, dealwhile_fun, &mydatainfo)) != 0){
        // debug_log("pthread_create_th_deal error", strerror(err));
        printf("revfromerror=%s\n", strerror(err));


    }
    
    int tot_num = 0;
     
    while(1){
        // printf("top=%d\n", mydatainfo.top);
        // if(is_empty(&mydatainfo) != 0){
        //     pthread_t th_deal;
        //     if((err = pthread_create(&th_deal, &attr, dealdata_fun, &mydatainfo)) != 0){
        //         perror("pthread create error");
        //     }
        // }
        int rev_num = 0;
        len = sizeof(addr_clie);
        n = recvfrom(s, &rev_num, sizeof(int), 0, (struct sockaddr*)&addr_clie, &len);  
        if(n <= 0){
            printf("revfromerror=%s\n", strerror(errno));
            return 1;
        }
        if(n > 0){
            push(&mydatainfo, rev_num);
            //printf("tot_g=%d\n", mydatainfo.top);
        }

    } 
};




void sig_int(int num)
{
    close_sql();
    printf("testover\n");
    printf("mydatainfo_top=%d\n", mydatainfo.top);
    int n = 0;
    for(n=0 ; n < mydatainfo.top; n++){
        printf("plist[%d]:%d\n", n, mydatainfo.plist[n]);
    }
    




    exit(1);
};

void set_sockopt(int s)
{
    int rc ;
    socklen_t optlen;
    // n = setsockopt(s,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
    // int rcv_buf_size = 0;//1000 * 1024;  
    // n = getsockopt(s, SOL_SOCKET, SO_RCVBUF, &rcv_buf_size, &optlen); 
    // printf("rcv=%d\n", rcv_buf_size);
    // return 1;
    // setsockopt(s, SOL_SOCKET, SO_RCVBUF, &rcv_buf_size, sizeof(int)); 
}

int main()
{
    int s;
    struct sockaddr_in addr_serv, addr_clie;
    char buff[1025];
    socklen_t len;
    int n;
    int flag;
    pid_t pid;
    signal(SIGINT, sig_int);
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s < 0){
        debug_log("socket error", strerror(errno));
        return -1;
    }
    memset(&addr_serv, 0, sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);
    // addr_serv.sin_addr.s_addr = inet_addr("10.66.84.56");
    addr_serv.sin_port = htons(PORT_SERV);
    flag = init_mysql();
    if(flag != 0){
        debug_log("init_mysql error", "null");
        return -1;
    }
    flag = bind(s, (struct sockaddr*)&addr_serv, sizeof(addr_serv));
    if(flag < 0){
        debug_log("bind error", strerror(errno));
        return -1;
    }
    handle_connect(s);
    return 1;
}
