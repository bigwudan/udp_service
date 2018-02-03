#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main()
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p = gmtime(&timep);

    printf("%d-%d-%d\n", (1900+p->tm_year), (1+p->tm_mon), p->tm_mday);

    printf("%d-%d\n", 24 - p->tm_hour, p->tm_min);

    char filename[20] = {0};

    sprintf(filename, "%d-%d-%d-%d-%d",(1900+p->tm_year), (1+p->tm_mon), p->tm_mday,  24 - p->tm_hour, p->tm_min);

    printf("file=%s\n", filename);

    return 0;   
}
