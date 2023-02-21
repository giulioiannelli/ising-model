#include <stdio.h>
#include <stdlib.h>




typedef struct mine {
    int *a;
} mia;




void sum_arr(int t, double *a, double *b)
{
    for (int tt = 0; tt < t; tt++)
    {
        printf("a%d\n", tt);
        *(a + tt) += *(b + tt);
    }
}

void sum_one(int t, double *a, double *b)
{
    for (int tt = 0; tt < t; tt++)
    {
        *a += *(b + tt);
    }
}

void mycal(double **a)
{
    *a = calloc(10, sizeof(**a));
}

void mycal2(mia *prova)
{
    prova->a = calloc(10, sizeof(*prova->a));
}

int main () 
{
    double *a, *b;
    mia prova;
//     a = calloc(10, sizeof(*a));
//     b = calloc(10, sizeof(*b));
//     for (int i = 0; i < 10; i++)
//     {
//         b[i] = i+1;
//     }
//     printf("--->1\n");
//     sum_arr(10, a, b);
//     printf("--->2\n");
//     for (int i = 0; i < 10; i++)
//     {
//         printf("%lf\n", a[i]);
//     }
//     sum_one(10, &a[0],b);
// printf("%lf\n", a[0]);


//     free(a);
//     free(b);

    mycal(&a);
    a[8] += 11;
    printf("a[8] = %lf\n", a[8]);
    mycal2(&prova);
    prova.a[8] += 11;
    printf("prova.a[8] = %d\n", prova.a[8]);
}