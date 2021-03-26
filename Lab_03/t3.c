/*
 * Fill in the "producer" function to satisfy the requirements 
 * set forth in the lab description.
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 
 * the total number of consumer threads created.
 * each consumer thread consumes one item
 */
#define TOTAL_CONSUMER_THREADS 100

/* This is the number of items produced by the producer each time. */
#define NUM_ITEMS_PER_PRODUCE  10

/*
 * the two functions for the producer and 
 * the consumer, respectively
 */
void *producer(void *);
void *consumer(void *);


/********** global variables begin *******/

pthread_mutex_t mut;
pthread_cond_t producer_cv; 	
pthread_cond_t consumer_cv;
int supply = 0;  /* inventory remaining */

/* 
 * Number of consumer threads that are yet to consume items.  Remember
 * that each consumer thread consumes only one item, so initially, this 
 * is set to TOTAL_CONSUMER_THREADS
 */
int num_cons_remaining = TOTAL_CONSUMER_THREADS;

/************** global variables end ****************************/



int main(int argc, char * argv[])
{
  pthread_t prod_tid;
  pthread_t cons_tid[TOTAL_CONSUMER_THREADS];
  int thread_index[TOTAL_CONSUMER_THREADS];
  int i;

  /********** initialize mutex and condition variables ***********/
  pthread_mutex_init(&mut, NULL);
  pthread_cond_init(&producer_cv, NULL);
  pthread_cond_init(&consumer_cv, NULL);
  /***************************************************************/


  /* create producer thread */
  pthread_create(&prod_tid, NULL, producer, NULL);

  /* create consumer thread */
  for (i = 0; i < TOTAL_CONSUMER_THREADS; i++)
  {
	thread_index[i] = i;
    pthread_create(&cons_tid[i], NULL, consumer, (void *)&thread_index[i]);
  }

  /* join all threads */
  pthread_join(prod_tid, NULL);
  for (i = 0; i < TOTAL_CONSUMER_THREADS; i++)
    pthread_join(cons_tid[i], NULL);

  printf("All threads complete\n");

  return 0;
}




/***************** Consumers and Producers ******************/

void *producer(void *arg)
{
  int producer_done = 0; 
  // not really sure if I should lock/unlock in or out of the while loop
  // can't really tell if it makes a difference
  pthread_mutex_lock(&mut);
  while (!producer_done)
  {
    // pthread_mutex_lock(&mut);
    /* TODO: fill in the code here */
    while (supply > 0){ // while there's still a supply, wait
      pthread_cond_wait(&producer_cv, &mut);
    }

    printf("producer thread produces %d items\n", NUM_ITEMS_PER_PRODUCE);
    fflush(stdout);
    supply = 10; // item "production" occurs here
    pthread_cond_broadcast(&consumer_cv); // tell consumers that more items have been produced
    if (num_cons_remaining < 1){//we're out of consumers, exit the program
      producer_done = 1;
    }
    // pthread_mutex_unlock(&mut);
  }
  printf("producer exiting while loop\n");
  pthread_mutex_unlock(&mut);
  return NULL;
}


void *consumer(void *arg)
{
  int cid = *((int *)arg);

  pthread_mutex_lock(&mut);
  while (supply == 0)
    pthread_cond_wait(&consumer_cv, &mut);

  printf("consumer thread id %d consumes an item\n", cid);
  fflush(stdout);

  supply--;
  if (supply == 0)
    pthread_cond_broadcast(&producer_cv);

  num_cons_remaining--;
  printf("%d items remaining, %d consumers remaining\n",supply, num_cons_remaining);
  pthread_mutex_unlock(&mut);

  return NULL;
}

