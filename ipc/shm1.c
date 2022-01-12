// #include "header.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main(void)
{
	const int id = shmget(2, 50, IPC_CREAT | 0666);
	perror("shmget");
	printf("id = %d\n", id);
	char * const p = shmat(id, 0, 0);
	printf("give str: ");
	scanf("%s", p);

	return 0;
}
