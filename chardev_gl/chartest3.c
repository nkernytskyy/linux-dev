#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


const char * dev_file = "/dev/chardev";
const size_t maxlen = 80;


int main(void);
void dev_read(const int fd, char * buf, const size_t len);
void dev_write(const int fd, const char * buf, const size_t len);


int main(void) {
	char * buffer = (char *) malloc(maxlen * sizeof(typeof(buffer[0])));
	if (buffer == NULL) {
		return -1;
	}

	int fd = 0;
	if (access(dev_file, F_OK) == 0) {
		fd = open(dev_file, O_RDWR);
	} else {
		fprintf(stderr, "\nError opening file:\t%s\n", dev_file);
		return -1;
	}

	buffer = "Stroustrup, Sutter, Meyers, Alexandresku\0";
	dev_write(dev_file, buffer, strlen(buffer));
	dev_read(dev_file, buffer, maxlen);

	buffer = "some text here\0";
	dev_write(dev_file, buffer, strlen(buffer));
	dev_read(dev_file, buffer, maxlen);

	close(fd);
	return 0;
}


void dev_read(const int fd, char * buf, const size_t len) {
	read(fd, (void*)buf, len);
	fprintf(stdout, "\nChar Device read\t:\t%s\n", buf);
}


void dev_write(const int fd, const char * buf, const size_t len) {
	write(fd, (void*)buf, len);
	fprintf(stdout, "\nChar Device write\t:\t%s\n", buf);
}

