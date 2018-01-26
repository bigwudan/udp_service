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

#include "mysql_connect.h"

#define PORT_SERV 8911

static void handle_request(char *buff, int n)
{
    printf("test=%s,pid=%d\n", buff, (int)getpid());
    char write[1204]={'\0'};
    sprintf(write, "insert into test_udp (key_value) values('%s');", buff);
    insert(write);
    
};

static int handle_connect(int s)
{
    struct sockaddr_in addr_clie;
    socklen_t len;
    int n;
    pid_t pid;    
    while(1){
        printf("wudan\n");  
        char buff[1025] = {'\0'};
        len = sizeof(addr_clie);
        n = recvfrom(s, buff, 1025, 0, (struct sockaddr*)&addr_clie, &len);  
        sendto(s, buff, n, 0, (struct sockaddr*)&addr_clie, len);
        if(n > 0){
            pid = fork();
            if(pid > 0){
                signal(SIGCLD, SIG_IGN);
            }else{
                handle_request(buff, n);
                return 0;
            }
        }
        
    } 
};




void sig_int(int num)
{
    printf("testover\n");
    exit(1);
};

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
    int lentest = 0;
    handle_connect(s);
    return 1;
}
