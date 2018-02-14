#ifndef _STACK_DATA_H
#define _STACK_DATA_H
//类型
struct datainfo{
    int *plist;
    int top;
    pthread_mutex_t mutex;
};
extern int is_empty(struct datainfo *pdatainfo);
extern int is_full(struct datainfo *pdatainfo);
//初始化
extern int init(struct datainfo *pdatainfo);
//push
extern void push(struct datainfo *pdatainfo, int data);
extern int pop(struct datainfo *pdatainfo);
extern int destory(struct datainfo *pdatainfo);
#endif  