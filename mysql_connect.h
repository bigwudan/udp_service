#ifndef _MYSQL_CONNECT_H
#define _MYSQL_CONNECT_H
extern int insert(const char *);
extern int sql_test();
extern void debug_log(const char *, const char *);
extern int insertsql(const char *);
extern int close_sql();
#endif  