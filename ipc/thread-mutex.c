#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

static int shared_int = 0;
static pthread_mutex_t mutex;

void * thread_1(void * p)
{
    while(true) {
        pthread_mutex_lock(&mutex);
        ++shared_int;
        pthread_mutex_unlock(&mutex);

        printf("thread id %ld sets %d\n", pthread_self(), shared_int);
        sleep(1);
    }
    // pthread_exit(pthread_self());
}

void * thread_2(void * p)
{
    while(true) {
        pthread_mutex_lock(&mutex);
        ++shared_int;
        pthread_mutex_unlock(&mutex);

        printf("thread id %ld sets %d\n", pthread_self(), shared_int);
        sleep(1);
    }
    // pthread_exit(pthread_self());
}

int main(void)
{
    pthread_mutex_init(&mutex, NULL);
    pthread_t t_1;
    pthread_t t_2;
    pthread_create(&t_1, NULL, thread_1, NULL);
    pthread_create(&t_2, NULL, thread_2, NULL);
    // while(true) {

    // }
    void** t_1_ret = NULL;
    void** t_2_ret = NULL;
    pthread_join(t_1, t_1_ret);
    pthread_join(t_2, t_2_ret);
}
