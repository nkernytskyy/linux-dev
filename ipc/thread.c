#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void * thread_fn(void * p)
{
    printf("thread id = %ld\n", pthread_self());
    sleep(3);
    pthread_exit("exit thread\n");
}

int main(void)
{
    pthread_t t1;
    pthread_create(&t1, NULL, thread_fn, NULL);
    printf("main func = %lu\n", t1);
    
    char * a;
    pthread_join(t1, (void*)&a);
    printf("thread farewell: %s\n", a);
}
