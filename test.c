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

//类型
struct datainfo{
    int *plist;
    int top;

};

int maxnum = 10;

int is_empty(struct datainfo *pdatainfo)
{
    if(pdatainfo->top == 0){
        return 0;     
    }else{
        return pdatainfo->top;
    }
}


int is_full(struct datainfo *pdatainfo)
{
    if(pdatainfo->top >= maxnum){
        return -1;
    }else{
        return 0;
    }
}

//初始化
int init(struct datainfo *pdatainfo)
{
    pdatainfo->plist = malloc(maxnum*sizeof(int));
    if(pdatainfo->plist == NULL){
        return -1;
    }
    pdatainfo->top = 0;
    return 1;
}

//push
void push(struct datainfo *pdatainfo, int data)
{
    if(is_full(pdatainfo) == 0){
        pdatainfo->plist[pdatainfo->top] = data;
        pdatainfo->top++;
    }
}

int pop(struct datainfo *pdatainfo)
{
    if(is_empty(pdatainfo)){
        int data = 0;
        data = pdatainfo->plist[pdatainfo->top-1];
        pdatainfo->top --;
        return data;
    }
    return -1;
}



int destory(struct datainfo *pdatainfo)
{
    pdatainfo->top = 0;
    free(pdatainfo->plist);
    pdatainfo->plist = NULL;
}

int main()
{

    int flag = 0;        
    struct datainfo mydatainfo;
    
    flag = init(&mydatainfo);

    push(&mydatainfo, 1);
    push(&mydatainfo, 2);
    

    // printf("plist-1=%d\n", mydatainfo.plist[0]);
    // printf("plist-2=%d\n", mydatainfo.plist[1]);
    // printf("top=%d\n", mydatainfo.top);

    // flag = pop(&mydatainfo);
    printf("popnum1=%d\n", pop(&mydatainfo));
    printf("popnum2=%d\n", pop(&mydatainfo));
    destory(&mydatainfo);
    return 1;

}
