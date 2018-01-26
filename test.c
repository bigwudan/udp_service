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



int main()
{
    insert("insert into test_udp (key_value) values ('wudan');");
    return 1;
}
