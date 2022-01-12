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
	mkfifo("f", 0666);
	perror("mkfifo (r)");

	printf("\n\tpid = %d\n\n", getpid());
	
	#define MAX_LEN (20)
	char  a[MAX_LEN] = { 0 };

	int fd = open("f", O_RDONLY);
	while (true) {
		if (read(fd, a, sizeof(a)) > 0) {
			printf("given msg: %s\n", a);
		}
	}
	close(fd);

	return 0;
}
