#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ANSI-color-codes.h"


#define PROMPT_FLAG "-p"
#define DEFAULT_PROMPT "308sh>"
#define TOKEN_DELIMITERS " \t\r\n\a"
#define BUFFSIZE 4096

int shell_exit(char **arg_arr);
int shell_pid(char **arg_arr);
int shell_ppid(char **arg_arr);
int shell_cd(char **arg_arr);
int shell_pwd(char **arg_arr);

//executes non-builtin command
int shell_exe(char **arg_arr){
    pid_t pid, wpid;
    int status;
    char *cmd = arg_arr[0];
    char *exit_color;
    pid = fork();
    if(pid == 0){
        printf("%s\n",cmd );
        if(execvp(cmd, arg_arr) == -1){
            printf("%s308sh: Command \"%s\" not recognized.%s\n",RED,arg_arr[0],RESET);
        }
        exit(1);
    }
    else if(pid < 0){
        perror("308sh");
    }
    else {
        printf("%s[%d] %s%s\n",GRN, pid, arg_arr[0], RESET);
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        
    }
    exit_color = (status == 0)?GRN:RED;
    printf("%s[%d] %s Exit %d%s\n",exit_color, pid, cmd, status, RESET);
    return 1;
} 

int shell_command(char **arg_arr){
    char *cmd = arg_arr[0];
    
    if(arg_arr[0] == NULL){
        return 1;
    }

    // I know it's ugly, but whatever
    if(strcmp(cmd, "exit")==0){return shell_exit(arg_arr);}
    if(strcmp(cmd, "cd")==0){return shell_cd(arg_arr);}
    if(strcmp(cmd, "pid")==0){return shell_pid(arg_arr);}
    if(strcmp(cmd, "ppid")==0){return shell_ppid(arg_arr);}
    if(strcmp(cmd, "pwd")==0){return shell_pwd(arg_arr);}

    // Input wasn't a builtin command
    return shell_exe(arg_arr);
}

char **shell_parse_line(char *line){
    int buff_size, pos = 0;
    char **token_array = malloc(buff_size * sizeof(char*));
    char *curr_token;

    // Get the first token from the line, based on the delimiters constant
    curr_token = strtok(line, TOKEN_DELIMITERS);
    while(curr_token != NULL){
        //Store the current token in token_array at index pos
        token_array[pos] = curr_token;
        pos++;

        //Update the size of token_array in case pos gets larger than the BUFFSIZE constant
        if(pos >= BUFFSIZE){
            buff_size += BUFFSIZE;
            token_array = realloc(token_array, buff_size * sizeof(char*));
        }
        // From the man page:
        // "In each subsequent call that should parse the same string, str must be NULL."
        curr_token = strtok(NULL, TOKEN_DELIMITERS);
    }
    // Add the terminating null byte to the end of the token array, since our loop won't grab it
    token_array[pos] = '\0';
    return token_array;

}

char *shell_read_line(){
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = getline(&line, &len, stdin);

    // if failure to read line (including EOF)
    if(nread == -1){
        if(feof(stdin)){ // EOF
            exit(0);
        }
        else{ // actual error!
            perror("readline");
            exit(1);
        }
    }
    return line;
}

void shell_loop(char *prompt){
    char  *curr_line;
    char **curr_args;
    int    curr_status;
    
    if(!prompt){
        prompt = DEFAULT_PROMPT;
    }

    do {
        printf("%s ",prompt);
        curr_line   = shell_read_line(); //Read the 
        curr_args   = shell_parse_line(curr_line);
        curr_status = shell_command(curr_args);
    } while (curr_status);
}

// Helper function to parse the command line inputs,
// and return the next argument after the prompt flag ("-p")
char *get_prompt(int arg_count, char *arg_arr[]){
    int i, prompt_index;
    for(i=0;i<arg_count;i++){
        if(strcmp(arg_arr[i], PROMPT_FLAG)==0){
            return arg_arr[i+1];
        }
    }
    return NULL;
}

int main(int argc, char *argv[]){
    // "infinite" loop
    char *desired_prompt = get_prompt(argc, argv);
    shell_loop(desired_prompt);
    return 0;
}