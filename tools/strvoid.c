#include <stdio.h>

void printonstr(char *a)
{
    sprintf(a, "atradeprova\n");
}

int main() {
    char a[100];
    printonstr(a);
    printf("print on: %s", a);
}