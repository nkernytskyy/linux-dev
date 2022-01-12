#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define FIFO_NAME "myfifo"

int main(void)
{
	char buf[256];
	int num = 0;
	int fd 	= 0;

	mkfifo(FIFO_NAME, 0666);
	perror("mkfifo");

	if (fork() == 0) {
		printf("child  - waiting for readers...\n");
		(fd = open(FIFO_NAME, O_WRONLY));
		perror("child  - open");
		
		printf("child  - got a reader -- type some stuff: ");
		while (fgets(buf, sizeof(buf), stdin), !feof(stdin)) {
			num = write(fd, buf, strlen(buf));
			if (num < 0) {
				perror("child  - write");
			} else {
				printf("child  - wrote %d bytes\n", num);
			}
		}

		close(fd);
		exit(0);
	} else {
		printf("parent - waiting for writers...\n");
		(fd = open(FIFO_NAME, O_RDONLY));
		perror("parent - open");
		
		printf("parent - got a writer\n");
		do {
			num = read(fd, buf, sizeof(buf));
			if (num < 0) {
				perror("parent - read");
			} else {
				buf[num] = '\0';
				printf("parent - read  %d bytes: %s\n", num, buf);
			}
		} while (num > 0);

		close(fd);
		wait(0);
	}

	unlink(FIFO_NAME);

	return 0;
}

// origin:
// https://stackoverflow.com/questions/2935557/c-named-pipe-fifo-parent-process-gets-stuck
