#include <stdio.h>
#include <stdlib.h>


const char * dev_file = "/dev/chardev";
char * buffer;
const size_t buflen = 80;


int main(void);
void dev_read(const char * file, char * buf, const int len);
void dev_write(const char * file, char * buf, const int len);


int main(void) {
	buffer = (char *) malloc(buflen * sizeof(typeof(buffer[0])));
	if (buffer == 0) {
		return -1;
	}

	dev_read(dev_file, buffer, buflen);
	buffer = "Stroustrup\0";
	//buffer = "Stroustrup, Sutter, Meyer, Alexandresku\0";
	dev_write(dev_file, buffer, buflen);
	dev_read(dev_file, buffer, buflen);
	buffer = "some text buffer here\0";
	dev_write(dev_file, buffer, buflen);
	dev_read(dev_file, buffer, buflen);

	return 0;
}


void dev_read(const char * file, char * buf, int len) {
	FILE *fd = fopen(file, "r");
	if (fd == NULL) {
		fprintf(stderr, "\nError read:\t%s\n", file);
	} else {
		fgets(buf, len, (FILE*)fd);
		fprintf(stdout, "\nChar Device read:\t%s\n", buf);
	}

	fclose(fd);
}


void dev_write(const char * file, char * buf, const int len) {
	FILE *fd = fopen(file, "w");
	if (fd == NULL) {
		fprintf(stderr, "\nError open for writing:\t%s\n", file);
	} else {
		fputs(buf, (FILE*)fd);
		fprintf(stdout, "\nChar Device write:\t%s\n", buf);
	}

	fclose(fd);
}
