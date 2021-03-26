#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char msg[100];
void my_alarm();

int main(int argc, char * argv[]){
    int time;
    if(argc < 3){
        printf("Not enough parameters\n");
        exit(1);
    }
    time = atoi(argv[2]);
    printf("Time: %d\n", atoi(argv[2]));
    strcpy(msg, argv[1]);
    signal(SIGALRM, my_alarm);
    alarm(time);
    printf("Entering infinite loop\n");
    while (1){ sleep(10); }
    printf("Can't get here\n");
}

void my_alarm(){
    printf("%s\n",msg);
    exit(0);
}