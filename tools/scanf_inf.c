#include <stdio.h>

void myscanf(int a)
{
    scanf("%d", &a);
}

int main()
{
    int a = 2;
    myscanf(a);
    printf("a = %d\n", a);
}