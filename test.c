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

void debug_log(const char *pcontent)
{
    syslog(LOG_MAIL | LOG_EMERG, "udp: %s \n", pcontent);   
    closelog();   
}

int main()
{
    debug_log("test");

}
