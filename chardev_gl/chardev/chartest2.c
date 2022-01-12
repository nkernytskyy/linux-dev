#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


const char * dev_file = "/dev/chardev";
const size_t maxlen = 80;


int main(void);
void dev_read(const char * file, char * buf, const size_t len);
void dev_write(const char * file, const char * buf, const size_t len);


int main(void) {
	char * buffer = (char *) malloc(maxlen * sizeof(typeof(buffer[0])));
	if (buffer == NULL) {
		return -1;
	}

	dev_read(dev_file, buffer, maxlen);
	
	buffer = "Stroustrup, Sutter, Meyers, Alexandresku\0";
	dev_write(dev_file, buffer, strlen(buffer));
	dev_read(dev_file, buffer, maxlen);
	
	buffer = "some text here\0";
	dev_write(dev_file, buffer, strlen(buffer));
	dev_read(dev_file, buffer, maxlen);

	return 0;
}


void dev_read(const char * dev_file, char * buf, const size_t len) {
	int fd = 0;
	if (access(dev_file, F_OK) == 0) {
		fd = open(dev_file, O_RDONLY);
		read(fd, (void*)buf, len);
		fprintf(stdout, "\nChar Device read:\t%s\n", buf);
        } else { 
		fprintf(stderr, "\nError reading:\t%s\n", dev_file);
	}
	close(fd);
}


void dev_write(const char * dev_file, const char * buf, const size_t len) {
	int fd = 0;
	if (access(dev_file, F_OK) == 0) {
		fd = open(dev_file, O_APPEND);
		write(fd, (void*)buf, len);
		fprintf(stdout, "\nChar Device write:\t%s\n", buf);
	} else {
		fprintf(stderr, "\nError open for writing:\t%s\n", dev_file);
	}
	close(fd);
}
