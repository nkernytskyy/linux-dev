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

#define MAX_LEN 20
#define FIFO_FD "t_fifo"

void * thread_wr(void * p)
{
    int fd = open(FIFO_FD, O_WRONLY | O_CREAT, 0666);
    char wr_buf[MAX_LEN];
    while(true) {
        printf("enter msg: ");
        scanf("%s", wr_buf);
        write(fd, wr_buf, strlen(wr_buf)+1);
    }
    // pthread_exit(pthread_self());
}

void * thread_rd(void * p)
{
    int fd = open(FIFO_FD, O_RDONLY | O_CREAT, 0666);
    char rd_buf[MAX_LEN];
    while(true) {
        if (read(fd, rd_buf, MAX_LEN) > 0) {
            printf("read  msg: %s\n", rd_buf);
        }
    }
    // pthread_exit(pthread_self());
}

int main(void)
{
    remove(FIFO_FD);

    pthread_t t_wr;
    pthread_t t_rd;
    pthread_create(&t_wr, NULL, thread_wr, NULL);
    pthread_create(&t_rd, NULL, thread_rd, NULL);
    // while(true) {

    // }
    void** t_wr_ret = NULL;
    void** t_rd_ret = NULL;
    pthread_join(t_wr, t_wr_ret);
    pthread_join(t_rd, t_rd_ret);
}
