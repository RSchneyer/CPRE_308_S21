#include <stdio.h>
#include <signal.h>

int main(){
    int i=0;
    int child_pid = fork();
    if(child_pid==0){
        while(1){
            i++;
            printf("Child at count %d\n",i);
            usleep(100000);
        }
    }
    else {
        printf("Parent sleeping\n");
        sleep(10);
        kill(child_pid, SIGTERM);
        printf("Child has been killed. Waiting for it... ");
        wait(NULL);
        printf("done.\n");
    }
    return 0;
}