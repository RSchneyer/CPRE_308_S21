#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define PROMPT_FLAG "-p"
#define DEFAULT_PROMPT "308sh> "
#define TOKEN_DELIMITERS " \t\r\n\a"
#define BUFFSIZE 4096


int shell_cd(char **arg_arr){
    if(arg_arr[1] == NULL){
        fprintf(stderr,"308sh: expected argument to \"cd\"\n");
    }
    else {
        if(chdir(arg_arr[1])!=0){
            perror("308sh");
        }
    }
    return 1;
}

int shell_execute(char **arg_arr){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0){
        if(execvp(arg_arr[0], arg_arr) == -1){
            perror("308sh");
        }
        exit(1);
    }
    else if(pid < 0){
        perror("308sh");
    }
    else {
        printf("[%ld] %s\n", pid, arg_arr[0]);
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        
    }
    return 1;
} 

char **shell_parse_line(char *line){
    int buff_size, pos = 0;
    char **token_array = malloc(buff_size * sizeof(char*));
    char *curr_token;

    curr_token = strtok(line, TOKEN_DELIMITERS);
    while(curr_token != NULL){
        token_array[pos] = curr_token;
        pos++;

        if(pos >= BUFFSIZE){
            buff_size += BUFFSIZE;
            token_array = realloc(token_array, buff_size * sizeof(char*));
        }
        curr_token = strtok(NULL, TOKEN_DELIMITERS);
    }
    token_array[pos] = NULL;
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
        printf(prompt);
        curr_line   = shell_read_line();
        curr_args   = shell_parse_line(curr_line);
        curr_status = shell_execute(curr_args);
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