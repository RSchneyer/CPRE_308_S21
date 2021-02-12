# Lab 2 Report
Name: Reid Schneyer

Section: 9

University ID: 944014098
## Summary
Summary goes here

## Lab Questions
### 3.1
#### In the report, include the relevant lines from `ps -l` for your programs, and point out the following items:

* process name
* process state (decode the letter!)
* process ID (PID)
* parent process ID (PPID)

![3.1 ps -l](imgs/3_1_ps_1.png)

*Name: three_one; State: Interruptable Sleep; PID: 140, 141; PPID: 111 for both*

#### Repeat this experiment and observe what changes and doesn't change
![3.1 ps -l v2](imgs/3_1_ps_2.png)

*PID changes. Name, State, and PPID remain the same*
#### Find out the name of the process that started your programs (the parent). What is it, and what does it do?
I found the parent with `ps -p <PPID> -o comm=`

![3.1 parent process](imgs/3_1_pp.png)

*Bash is the default shell, and it executes commands from the user or from a file.*

## 3.2
#### Include the output from the program.
![3.2 program output](imgs/3_2_output.png)
#### "Draw" the process tree (the nodes should be PIDs)
![3.2 process tree](imgs/3_2_proc_tree.png)
#### Explain how the tree was built in terms of the program code
Node 226 is the bash process, and running the program creates its child, proc 281. Process 281 then calls the first `fork()`, which creates its child, proc 282. The process 282 calles the next `fork()`, which creates proc 283 as a child. Proc 283 then returns, and proc 281 calls the second `fork()`, which creates proc 284 as a child of 281. Proc 284 returnsm and then the program calls the `usleep()`, `printf()`,  and `sleep()` functions, which create no further processes.
#### Try the program again without `sleep(2)`. Explain what happens when the sleep statement is removed.  You should see processes reporting a parent PID of 1.  Redirecting output to a file may interferewith this, and you may need to run the program multiple times to see it happen.
![3.2 no sleep](imgs/3_2_nosleep.png)

*When the* `sleep()` *statement is removed, the parent process doesn't wait for the child process to complete, and exits before the child. Since the child is now an orphan, it gets adopted by the* `init` *process (PID=1)* 
## 3.3) The `fork()` syscall, continued
#### Include the (completed) program and its output.
```c
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
```
![3.3 output](imgs/3_3_out.png)
#### Speculate why it might be useful to have fork return different values to the parent and child. What advantage does returning the child’s PID have?  Keep in mind that often the parent and child processesneed to know each other’s PID.
`fork()` *returns the child's PID to the parent, since the child can already access its own PID and PPID with* `getpid()` *and* `getppid()`.
### 3.4 Time Slicing
#### Include small (but relevant) sections of the output
\<INSERT HERE>
#### Make some observations about time slicing. Can you find any output that appears to have been cut off? Are there any missing parts? What’s going on(mention the kernel scheduler)?
\<INSERT HERE
### 3.5 Process synchronization using `wait()`
#### Explain the major difference between this experiment and experiment 4.  Be sure to look up whatwait does.
*In this experiment, the child process loop all the way from 0 to 499999 without any missing parts or interruptions, then the parent process does the same, again without any missing parts or interruptions*
### Signals using `kill()`
#### The program appears to have an infinite loop.  Why does it stop?
*The program eventually stops when the parent is done sleeping. Once the parent has finished sleeping, it uses* `kill()` *to kill the child process, ending the "infinite" loop*
#### From the definitions of sleep and usleep, what do you expect the child’s count to be just before it ends?
*I would expect the child's count to be 1000 just before it ends, since it waits for 0.01s each loop, and the parent is asleep for 10s.*
#### Why doesn't the child reach this count before it terminates?
*The child does not reach 1000 before it terminates because*
### 3.7 The `execve()` family of functions
#### Run the following program and explain the results.
*When the program is ran, it lists out the contents of the directory that it gets called from using the* `ls` *command.*
#### Under what conditions is the printf statement  executed?   Why  isn’t  it  always  executed? 
*The printf statement is executed if the command in* `execl()` *is unsuccessful. For example, changing the line to* 
```c
execl("/bin/cd", "cd dne", (char *)NULL);
``` 
*(where dne is a sub-directory that does not exist in the directory that the program is called from) will instead execute the printf statement.*
### 3.8 The return value of `main()`
#### What is the range of possible exit status values printed for the child process?
*\<INSERT ANSWER HERE>*
#### What is the signal value the child process uses to terminate itself, as determined and printed by the parent process?
*\<INSERT ANSWER HERE>*
#### When do you think the return value of `main()` would be useful? Hint: look at the commands `true` and `false`.
*\<INSERT ANSWER HERE>*