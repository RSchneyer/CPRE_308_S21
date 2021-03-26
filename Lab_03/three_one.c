#include <pthread.h>
#include <stdio.h>

void * thread1(void * ptr);
void * thread2(void * ptr);

int main(int argc, char **argv){
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    
    pthread_join(&t1, NULL);
    pthread_join(&t2, NULL);

    printf("Hello from the main thread\n");
}

void* thread1(void *ptr){
    sleep(5);
    printf("Hello from thread1\n");
}
void* thread2(void *ptr){
    sleep(5);
    printf("Hello from thread2\n");
}