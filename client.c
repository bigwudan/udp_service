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

#define PORT_SERV 8911



void sig_int(int num)
{
    printf("testover\n");
    exit(1);
};

int main()
{
    // insert("11insert into user(name) values('wudan1')");
    // return 1;
    int s;
    struct sockaddr_in addr_serv, addr_clie;
    char buff[123];
    socklen_t len;
    int n;
    int flag;
    pid_t pid;

    int num = 100;
    char str[25];


    signal(SIGINT, sig_int);

    int a = 0;
    for(a = 0; a < 1500; a++){
        pid = fork();
        if(pid < 0){
            return 0;
        }else if(pid == 0){
            break;
        }else{

            memset(str, 0, 25);
            s = socket(AF_INET, SOCK_DGRAM, 0);
            memset(&addr_serv, 0, sizeof(addr_serv));
            addr_serv.sin_family = AF_INET;
            addr_serv.sin_addr.s_addr = inet_addr("114.215.85.234");
            addr_serv.sin_port = htons(PORT_SERV);

            
            sprintf(str, " %d" , a);

            n = sendto(s, str, 25,0 , (struct sockaddr*)&addr_serv, sizeof(addr_serv));
            
            printf("n=%d\n",n);
            close(s);        
        }
    }

    // s = socket(AF_INET, SOCK_DGRAM, 0);


    // printf("s=%d\n", s);

    // memset(&addr_serv, 0, sizeof(addr_serv));
    // addr_serv.sin_family = AF_INET;
    // addr_serv.sin_addr.s_addr = inet_addr("114.215.85.234");
    // addr_serv.sin_port = htons(PORT_SERV);
    // buff = '1';
    // n = sendto(s, &buff, 1,0 , (struct sockaddr*)&addr_serv, sizeof(addr_serv));
        
    // printf("n=%d\n",n);
    // close(s);
    return 1;
}
