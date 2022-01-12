#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
  
sem_t mutex;
  
void* thread(void* arg)
{
    //wait
    sem_wait(&mutex);
    printf("\nEntered %ld\n", pthread_self());
  
    //critical section
    sleep(3);
      
    //signal
    printf("Exiting %ld\n\n", pthread_self());
    sem_post(&mutex);
}
  
int main(void)
{
    sem_init(&mutex, 0, 1);

    pthread_t t1;
    pthread_create(&t1,NULL,thread,NULL);
    
    sleep(1);

    pthread_t t2;
    pthread_create(&t2,NULL,thread,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    
    sem_destroy(&mutex);
    
    return 0;
}
