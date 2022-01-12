// #include "header.h"
#include <fcntl.h> 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
	#define MAX_LEN (20)
	char  a[MAX_LEN] = { 0 };

	mkfifo("f", 0666);
	perror("mkfifo");

	// int fd = open("f", O_RDWR);

	if (fork() == 0) {
		// child
		// printf("Child  PID = %d\n", getpid());
		char  a[MAX_LEN] = { 0 };
		while (true) {
			int fd = open("f", O_RDONLY);
			read(fd, a, sizeof(a));
			printf("given msg: %s", a);
			close(fd);
		}
	} else {
		// parent
		// printf("Parent PID = %d\n", getpid());
		char  a[MAX_LEN] = { 0 };
		while (true) {
			int fd = open("f", O_WRONLY);
			printf("enter msg: ");
			scanf("%s", a);
			write(fd, a, strlen(a)+1);
			close(fd);
		}
	}

	return 0;
}
