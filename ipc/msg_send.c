#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN (20)
struct msgbuf 
{
    long mtype; // Mtype > 0;
    char data[MAX_LEN];
};

int main(int argc, char ** argv)
{
    int id = 0;
    struct msgbuf v;

    id = msgget(3, IPC_CREAT | 0666);
    perror("msg_send");

    v.mtype = atoi(argv[1]);
    strcpy(v.data, argv[2]);

    printf("id = %d\n", id);
    msgsnd(id, &v, strlen(v.data)+1, 0);
}

// to check messague queue run:
// ipcs -q
