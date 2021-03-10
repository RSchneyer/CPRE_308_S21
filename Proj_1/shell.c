#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ANSI-color-codes.h"

#define PROMPT_FLAG "-p"
#define DEFAULT_PROMPT "308sh>"

int shell_exit(char **arg_arr);
int shell_pid(char **arg_arr);
int shell_ppid(char **arg_arr);
int shell_cd(char **arg_arr);
int shell_pwd(char **arg_arr);

char *get_prompt(int arg_count, char **arg_arr);
char **parse_input(char *usr_input, int *arg_count);
int shell_non_builtin(char **arg_arr, int arg_count);

// TODO - Use to store background processes
typedef struct proc {int pid; char *cmd;} Process;



int main(int argc, char **argv){
    //Set the prompt
    char *desired_prompt = get_prompt(argc, argv);

    char *cmd;
    char **usr_input_arr;
    char buffer[BUFSIZ];
    int bkgnd, status;


    // "Infinite" Loop
    do{
        // Prompt user
        printf("%s ", desired_prompt);

        // Get/Parse user input
        usr_input_arr = parse_input(fgets(buffer, BUFSIZ, stdin), &bkgnd);
        cmd = usr_input_arr[0];

        //Handle builtin commands (ugly, I know)
        if(strcmp(cmd, "exit")==0){ status = shell_exit(usr_input_arr); }
        else if(strcmp(cmd, "cd")==0){ status = shell_cd(usr_input_arr); }
        else if(strcmp(cmd, "pid")==0){ status = shell_pid(usr_input_arr); }
        else if(strcmp(cmd, "ppid")==0){ status = shell_ppid(usr_input_arr); }
        else if(strcmp(cmd, "pwd")==0){ status = shell_pwd(usr_input_arr); }
        // I added this for testing purposes, installing sl was a mistake lol
        else if(strcmp(cmd, "sl")==0){usr_input_arr[0] = "ls";}
        //Non-built in command
        else {
            status = shell_non_builtin(usr_input_arr, bkgnd);
        }
    }while(status);
    return 0;
}

// Hopefully self-explanitory, checks the main() args for the "-p" flag, and if it exists,
// returns the desired user prompt. Otherwise, it returns the default prompt
char *get_prompt(int arg_count, char **arg_arr){
    int i, prompt_index;
    for(i=0;i<arg_count;i++){
        if((strcmp(arg_arr[i], PROMPT_FLAG)==0)&&arg_arr[i+1]){
            return arg_arr[i+1];
        }
    }
    return DEFAULT_PROMPT;
}

// Parses the string returned from fgets, the bkgnd pointer arg is used as a flag
char **parse_input(char *usr_input, int *bkgnd){
    int pos = 0;
    // Allocate memory for array of tokens, BUFSIZ comes from stdio.h
    char **token_arr = malloc(BUFSIZ* sizeof(char*));
    char *curr_token; 

    // Get token from user input string, delimiters are space and newline
    curr_token = strtok(usr_input, " \n");
    // While there's still tokens to get in the string
    while(curr_token != NULL){
        //store the current token at position pos, increment pos, and get the next token
        token_arr[pos] = curr_token;
        pos++;
        // From the strtok man page:
        //  "In each subsequent call that should parse the same string, str must be NULL."
        curr_token = strtok(NULL, " \n");
    }
    //set the last token as the end of line character or whatever \0 is
    token_arr[pos] = '\0';

    //So we can check if the last element of token_arr is '&' later
    if(strcmp(token_arr[pos-1], "&")==0){
        *bkgnd = 1;
        token_arr[pos-1] = '\0';
    }
    else{
        *bkgnd = 0;
    }
    return token_arr;
}

// Executes any commands that aren't builtin to the shell
int shell_non_builtin(char **arg_arr, int bkgnd){
    pid_t child_pid, wpid;
    int status, execvp_ret_val;
    char *cmd = arg_arr[0];
    char *exit_color;

    // Check for background processes
    int wait_pid = waitpid(-1, &status, WNOHANG);
    if(wait_pid>0){
        printf("%s[%d] Exit %d%s\n",WHT,wait_pid,status,RESET);
    }

    child_pid = fork();
    if(child_pid == 0){ // We're in the child process
        execvp_ret_val = execvp(cmd, arg_arr); // CALL TO EXECVP IS HERE
        if(execvp_ret_val== -1){
            printf("%s308sh: Command \"%s\" not recognized%s\n", RED, cmd, RESET);
            exit(1);
        }
    }
    else if(child_pid < 0){//Creation of a child process was unsuccessful 
        printf("%s308sh: Error creating child process%s\n",RED,RESET);
    }
    else{//We're in the parent process
        printf("%s[%d] %s%s\n", GRN, child_pid, cmd, RESET);
        if(bkgnd==0){
            do{
                wpid = waitpid(child_pid, &status, WUNTRACED);
            }while(!WIFSIGNALED(status) && !WIFEXITED(status));
        }
    }
    exit_color = ((status == 0)||(bkgnd==1))?GRN:RED;
    if(bkgnd==0){
        printf("%s[%d] %s Exit %d%s\n", exit_color, child_pid, cmd, status, RESET);
    }
    return 1;
}