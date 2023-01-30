#include <stdio.h>
#include <time.h>

#define TTO 10

int main()
{
    time_t sec;
    sec = time(NULL);
    printf("%ld, %lu\n", sec, (unsigned long)(sec << sec));
}