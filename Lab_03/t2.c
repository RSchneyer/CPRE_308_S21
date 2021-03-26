/* t2.c
   synchronize threads through mutex and conditional variable 
   To compile use: gcc -o t2 t2.c -lpthread 
*/ 

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* hello();    // define two routines called by threads    
void* world();  
void* again();  //define new routine called by thread     	

/* global variable shared by threads */
pthread_mutex_t mutex;  		// mutex
pthread_cond_t done_hello, done_world; 	// conditional variables (added done_world)
int h_done = 0;      	// testing variables
int w_done = 0;         // added w_done, updated done to h_done

int main (){
    pthread_t tid_hello, tid_world, tid_again; // thread ids
	
    /*  initialize mutex and cond variable  */ 
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&done_hello, NULL); 
    pthread_cond_init(&done_world, NULL); // new cond var init
    
    pthread_create(&tid_hello, NULL, hello, NULL); //thread creation
    pthread_create(&tid_world, NULL, world, NULL); //thread creation 
    pthread_create(&tid_again, NULL, again, NULL); // new thread creation

    /* main waits for the two threads to finish */
    pthread_join(tid_hello, NULL);  
    pthread_join(tid_world, NULL);
    pthread_join(tid_again, NULL); //added

    printf("\n");
    return 0;
}

void* hello() {
    pthread_mutex_lock(&mutex);
    printf("hello ");
    fflush(stdout); 	// flush buffer to allow instant print out
    h_done = 1; //updated var name
    pthread_cond_signal(&done_hello);	// signal world() thread
    pthread_mutex_unlock(&mutex);	// unlocks mutex to allow world to print
}


void* world() {
    pthread_mutex_lock(&mutex);

    /* world thread waits until done == 1. */
    while(h_done == 0){ //added curly brackets for personal reasons
		pthread_cond_wait(&done_hello, &mutex);
    }
    printf("world ");
    fflush(stdout);
    w_done = 1; //set second done flag
    pthread_cond_signal(&done_world); //signal to new signal
    pthread_mutex_unlock(&mutex); // unlocks mutex
}

// pretty much the same as the original world() function,
// but with different variables for pthread stuff (and different print)
void* again(){
    pthread_mutex_lock(&mutex);
    while (w_done == 0){ 
        pthread_cond_wait(&done_world, &mutex);
    }
    printf("again ");
    fflush(stdout);
    pthread_mutex_unlock(&mutex);
}
