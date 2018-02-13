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
//#include "mysql_connect.h" //gcc -I/usr/include/mysql/  -L/usr/lib64/mysql/ -lmysqlclient -lz main.c mysql_connect.c -o testudpmain && ./testudpmain
#include "stack_data.h"

#define PORT_SERV 8911

struct thread_arg{
    int num;
    struct datainfo *pdatainfo;
};


void get_timestr(char *filename)
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep);
    sprintf(filename, "testlog//%d-%.2d-%.2d-%.2d:%.2d.log",(1900+p->tm_year), (1+p->tm_mon), p->tm_mday,  p->tm_hour, p->tm_min);
}


int debug_log(const  char *ptitle , const char *pcontent)
{
    return 1;
    openlog(ptitle, LOG_CONS | LOG_PID, 0);   
    syslog(LOG_USER | LOG_DEBUG, "dubug: %s \n", pcontent);   
    closelog();   
}


void* wdb_fun(void *arg)
{
    struct thread_arg *plist = (struct thread_arg *)arg;
    //printf("arg_num=%d\n", plist->num);
    push(plist->pdatainfo, plist->num);
    struct datainfo *pdata = plist->pdatainfo;
    free(plist);
    plist = NULL;
    // printf("pop->num=%d\n", pop(pdata));
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

void* dealdata_fun(void *arg)
{
    struct datainfo *pdata =  (struct datainfo *)arg;
    if(arg == NULL){


    }else{
        // printf("pop->num=%d\n", pop(pdata));
        pthread_mutex_lock(&pdata->mutex);
        int num = pop(pdata);
        if(num >= 0){
            FILE *fp = NULL;
            char filename[30] = {0};
            char buff[1200] = {'\0'};

            sprintf(buff,"%d\n", num);
            // printf("a=%s", buff);
            get_timestr(filename);
            fp = fopen(filename, "a+");
            fputs(buff, fp);                    
            fclose(fp);
        }
        pthread_mutex_unlock(&pdata->mutex);
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
    struct datainfo mydatainfo;
    flag = init(&mydatainfo);
    if(flag != 1){
        printf("error create datainfo");
        return 1;
    }
    setnonblocking(s);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); 
    while(1){
        // printf("top=%d\n", mydatainfo.top);

        if(is_empty(&mydatainfo) != 0){
            pthread_t th_deal;
            if((err = pthread_create(&th_deal, &attr, dealdata_fun, &mydatainfo)) != 0){
                perror("pthread create error");
            }
        }

        int rev_num = 0;
        len = sizeof(addr_clie);
        n = recvfrom(s, &rev_num, sizeof(int), 0, (struct sockaddr*)&addr_clie, &len);  
        if(n < 0){
            // printf("revfromerror=%s\n", strerror(errno));
        }
        if(n > 0){
            pthread_t th;
            struct thread_arg *pthread_arg = malloc(sizeof(struct thread_arg));
            pthread_arg->num = rev_num;                
            pthread_arg->pdatainfo = &mydatainfo;
            if((err = pthread_create(&th, &attr, wdb_fun, pthread_arg)) != 0){
                perror("pthread create error");
            }
            // pthread_attr_destroy(&attr);
        }

    } 
};




void sig_int(int num)
{
    printf("testover\n");
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
    memset(&addr_serv, 0, sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);
    // addr_serv.sin_addr.s_addr = inet_addr("10.66.84.56");
    addr_serv.sin_port = htons(PORT_SERV);
    flag = bind(s, (struct sockaddr*)&addr_serv, sizeof(addr_serv));
    printf("flag=%d\n", flag);
    printf("error1=%s\n", strerror(errno));
    handle_connect(s);
    return 1;
}
