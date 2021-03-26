#include <signal.h>
#include <stdio.h>

void my_routine();

int main(){
    signal(SIGINT, SIG_IGN);
    printf("Entering infinite loop\n");
    while(1){
        sleep(10);
    }
    printf("Can't get here\n");
}

void my_routine(){
    printf("Running my_routine()\n");
}