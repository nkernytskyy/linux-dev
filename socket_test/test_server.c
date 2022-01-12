#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SA struct sockaddr

const char * DEVICE_FILE = "/dev/chardev";
const size_t MAX_LEN = 80;
const uint16_t PORT = 8080;

int main(void);
void dev_read(const int fd, char * buf, const size_t len);
void dev_write(const int fd, const char * buf, const size_t len);
void send_recv(const int sockfd, const int fd);

int main(void) {
	int sockfd;
    int connfd;
	int len;
    struct sockaddr_in srv_addr;
    struct sockaddr_in cli_addr;

    // socket create and verify
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("SOCKET: creation ");
    if (sockfd == -1) {
        printf("failed\n");
        exit(0);
    } else {
        printf("OK\n");
	}
    bzero(&srv_addr, sizeof(srv_addr));

	// assign IP, PORT
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
	printf("\nSOCKET: binding ");
    if ((bind(sockfd, (SA*)&srv_addr, sizeof(srv_addr))) != 0) {
        printf("failed!\n");
        exit(0);
    } else {
        printf("OK\n");
	}

    // Now server is ready to listen and verification
	printf("\nSERVER: listening ");
    if ((listen(sockfd, 5)) != 0) {
        printf("failed!\n");
        exit(0);
	} else {
        printf("OK\n");
	}
    len = sizeof(cli_addr);

    // Accept the data packet from client and verification
	printf("\nSERVER: acccept ");
    connfd = accept(sockfd, (SA*)&cli_addr, &len);
    if (connfd < 0) {
        printf("failed!\n");
        exit(0);
    } else {
        printf("OK\n");
	}

	// open character device
	int fd = 0;
	if (access(DEVICE_FILE, F_OK) == 0) {
		fd = open(DEVICE_FILE, O_RDWR);
		printf("\nDEVICE: open: OK\n");
	} else {
		fprintf(stderr, "\nERROR opening device:\t%s\n", DEVICE_FILE);
		exit(0);
	}

	// read from Client, respond from Server, write to Device and verify
    send_recv(connfd, fd);

    close(sockfd);
	close(fd);
	return 0;
}


void dev_read(const int fd, char * buf, const size_t len) {
	read(fd, (void*)buf, len);
	fprintf(stdout, "\nDEVICE: read\t:\t%s\n", buf);
}


void dev_write(const int fd, const char * buf, const size_t len) {
	write(fd, (void*)buf, len);
	fprintf(stdout, "\nDEVICE: write\t:\t%s\n", buf);
}


void send_recv(const int sockfd, const int fd) {
    char buffer[MAX_LEN];
    int n;

	// infinite loop for chat
    for (;;) {
        bzero(buffer, MAX_LEN);

        // read and print the message from Client
        read(sockfd, buffer, sizeof(buffer));
        printf("\nFrom Client\t:\t%s", buffer);

		// check for exit/stop/quit/q command from Client
		if ((strncmp(buffer, "exit", 4)) == 0 ||
			(strncmp(buffer, "stop", 4)) == 0 ||
			(strncmp(buffer, "quit", 4)) == 0 ||
			(strncmp(buffer, "q", 1)) == 0)
		{
            printf("\nServer Exit...\n");
            break;
        }

		// write to Device
		dev_write(fd, buffer, strlen(buffer));
		dev_read(fd, buffer, MAX_LEN);

		// send Server acknowledgement response to Client
        bzero(buffer, MAX_LEN);
		strncpy(buffer, "+", MAX_LEN);
        write(sockfd, buffer, sizeof(buffer));
    }
}

