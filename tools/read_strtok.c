#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#define MAXCHAR 1000

int main(){

    FILE *fp;
    char row[MAXCHAR];
    char *token;

    fp = fopen("../res/config/.config_acf_compute.csv","r");

    fgets(row, MAXCHAR, fp);
    fgets(row, MAXCHAR, fp);

    token = strtok(row, ",");

    while(token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, ",");
    }

    

    return 0;

}