#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_LEN (50)
#define SEM_CNT (2)

int main(void)
{
    int shmid = shmget(2, MAX_LEN, IPC_CREAT | 0644);
    char * shmp = shmat(shmid, NULL, 0);
    bzero(shmp, MAX_LEN);

    int semid = semget(2, SEM_CNT, IPC_CREAT | 0644);
    
    struct sembuf v;
    v.sem_num = 1;
    v.sem_op  = 0;
    v.sem_flg = 0;

    if (fork()) {
        while (true) {
            printf("give msg: ");
            semop(semid, &v, 1);
            semctl(semid, 1, SETVAL, 1);
            scanf("%s", shmp);
            semctl(semid, 1, SETVAL, 0);
        }
    } else {
        while (true) {
            semop(semid, &v, 1);
            semctl(semid, 1, SETVAL, 1);
            if (strlen(shmp) > 0) {
                printf("read msg: %s", shmp);
                shmp[0] = '\0';  // nullify previous msg
            }
            semctl(semid, 1, SETVAL, 0);
        }
    }
}

// SysV IPC semaphore (semctl)
// POSIX semaphores (sem_init)
