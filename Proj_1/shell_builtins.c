#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>


// Function that handles the exit command
// Returns 0, which is a successful exit code
int shell_exit(char **arg_arr){return 0;}

//Function that handles pid command
int shell_pid(char **arg_arr){
    printf("Shell PID: %d\n", getpid());
}
// Function that handles the ppid command
int shell_ppid(char **arg_arr){
    printf("Shell's parent ID: %d\n", getppid());
}

// Function that handles the cd command
int shell_cd(char **arg_arr){
    if(arg_arr[1] == NULL){// 308sh> cd
        if(chdir(getenv("HOME"))!=0){
            perror("308sh");
        }
    }
    else {// 308sh> cd dir
        if(chdir(arg_arr[1])!=0){
            perror("308sh");
        }
    }
    return 1;
}
// Function that handles the pwd command
int shell_pwd(char **arg_arr){
    char pwd[PATH_MAX];
    if(getcwd(pwd, sizeof(pwd))!=NULL){
        printf("%s\n", pwd);
    }
    else{
        perror("getcwd() error");
        return 1;
    }
}