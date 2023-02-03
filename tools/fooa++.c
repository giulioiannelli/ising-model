#include <stdio.h>

void foo(int a)
{
    printf("a = %d\n", a);
}
int main() 
{
    int l;
    int a=0;
    int arr[10];
    for (int i =0; i < 10; i++)
    {
        arr[i] = i;
    }
    foo(a++);
    printf("a post = %d\n", a);
    printf("arr = %d, l = %d\n", arr[l=5], l);
    printf("arr = %d, l = %d, %d\n",l+(a=2), l, a);
}