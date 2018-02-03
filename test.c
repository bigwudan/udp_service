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




int main()
{
    openlog("testsyslog", LOG_CONS | LOG_PID, 0);   
    syslog(LOG_USER | LOG_DEBUG, "syslog test message generated in program %s \n");   
    closelog();   
    return 0;   
}
