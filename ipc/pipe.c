// #include "header.h"
#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

int main(void)
{
	int pipefd[2] = { 0, 0 };
	int ret = pipe(pipefd);

	if (ret < 0) {
		perror("pipe");
		return -1;
	}

	printf("\tp[0] = %d\n", pipefd[0]);
	printf("\tp[1] = %d\n", pipefd[1]);
	
	#define MAX_LEN (20)
	if (fork() == 0) {
		// child
		while (true) {
			sleep(1);
			static char b[MAX_LEN];
			printf("before read in child\n");
			read(pipefd[0], b, MAX_LEN);
			printf("after  read in child: %s\n", b);
		}
	} else {
		// parent
		while (true) {
			sleep(2);
			static char a[MAX_LEN];
			printf("enter msg:");
			scanf("%s", a);
			write(pipefd[1], a, strlen(a)+1);
		}
	}

	return 0;
}
