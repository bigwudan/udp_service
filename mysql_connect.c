/*
 ============================================================================
 Name        : connect.c
 Author      : 风飘无痕
 Version     :
 Copyright   : Your copyright notice
 Description : Connect Mysql
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "mysql_connect.h"
MYSQL *g_conn; // mysql 连接
MYSQL_RES *g_res; // mysql 记录集
MYSQL_ROW g_row; // 字符串数组，mysql 记录行
#define MAX_BUF_SIZE 1024 // 缓冲区最大字节数
const char *g_host_name = "localhost";
const char *g_user_name = "root";
const char *g_password = "";
const char *g_db_name = "test";
const unsigned int g_db_port = 3306;

void print_mysql_error(const char *msg) { // 打印最后一次错误
    if (msg)
        printf("%s: %s\n", msg, mysql_error(g_conn));
    else
        puts(mysql_error(g_conn));
}

int executesql(const char * sql) {
    /*query the database according the sql*/
    if (mysql_real_query(g_conn, sql, strlen(sql))) // 如果失败
        return -1; // 表示失败

    return 0; // 成功执行
}


int init_mysql() { // 初始化连接
    // init the database connection
    g_conn = mysql_init(NULL);
    
    /* connect the database */
    if(!mysql_real_connect(g_conn, g_host_name, g_user_name, g_password, g_db_name, g_db_port, NULL, 0)) // 如果失败
        return -1;

    // 是否连接已经可用
    if (executesql("set names utf8")) // 如果失败
       return -1;
    return 0; // 返回成功
}


int insert(const char *sql_str)
{
    int flag = 0;
    flag = init_mysql();
    printf("flag=%d\n", flag);
    print_mysql_error(NULL);
    flag = executesql(sql_str);
    mysql_close(g_conn); // 关闭链接
    return 1;
}

int sql_test()
{
    int flag = 0;
    flag = init_mysql();
    printf("flag=%d\n", flag);
}