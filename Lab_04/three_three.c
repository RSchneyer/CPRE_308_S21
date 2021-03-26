#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void div_by_zero_catch();

int main(){
    
    int a = 4;
    printf("Going to divide %d by 0\n", a);
    a = a/0;
    signal(SIGFPE, div_by_zero_catch);
}

void div_by_zero_catch(){
    printf("Caught a SIGFPE!\n");
    exit(1);
}