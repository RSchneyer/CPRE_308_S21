#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#define BUFFSIZE 4096

int shell_exit(char **arg_arr){return 0;}

int shell_pid(char **arg_arr){return 1;}

int shell_ppid(char **arg_arr){return 1;}

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