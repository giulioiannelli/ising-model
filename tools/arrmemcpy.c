#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUMBER 10000

int main()
{
    int *s;
    s = malloc(sizeof(*s) * NUMBER);
    *s = -1;
    clock_t begin = clock();
    // memset(s, *s, sizeof(*s) * NUMBER);
    for (int i = 1; i < NUMBER; i++)
    {
        *(s + i) = *s;
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    // for (int i = 0; i < NUMBER; i++)
    // {
    //     printf("i: %d s: %d\n", i, *(s+i));
    // }
    printf("time spent = %g\n", time_spent);
}