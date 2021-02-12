#include <stdio.h>
int main(){
    int ret;
    ret = fork();
    if(ret == 0){//This is the child process 
        printf("The child process ID is %d\n", getpid());
        printf("The child's parent process ID is %d\n", getppid());
    }
    else {//This is the parent process
        printf("The parent process ID is %d\n", getpid());
        printf("The parent's parent process ID is %d\n", getppid());
    }
    sleep(2);
    return 0;
}