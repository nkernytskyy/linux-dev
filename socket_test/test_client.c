#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define SA struct sockaddr

// maybe add custom/default IP as cmd arg
const char IP_ADDR_SRV[] = "127.0.0.1";
const size_t MAX_LEN = 80;
const uint16_t PORT = 8080;

int main(void);
void send_recv(const int sockfd);

int main(void)
{
    int sockfd;
    int connfd;
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
    srv_addr.sin_addr.s_addr = inet_addr(IP_ADDR_SRV);
    srv_addr.sin_port = htons(PORT);

    // connect the client socket to server socket
	printf("\nCLIENT: connection to Server:\t");
    if (connect(sockfd, (SA*)&srv_addr, sizeof(srv_addr)) != 0) {
        printf("failed!\n");
        exit(0);
    } else {
		printf("established.\n");
	}

    // function for chat
    send_recv(sockfd);

    // close the socket
    close(sockfd);
	printf("\nCLIENT: connection:\tclosed.\n");
}


void send_recv(const int sockfd) {
    char buffer[MAX_LEN];
    int n;

    for (;;) {
        // get user input
        bzero(buffer, sizeof(buffer));
		printf("Enter message to Server : ");
        n = 0;
        while ((buffer[n++] = getchar()) != '\n')
            ;
        write(sockfd, buffer, sizeof(buffer));

		// check for exit/stop/quit/q command
		if ((strncmp(buffer, "exit", 4)) == 0 ||
			(strncmp(buffer, "stop", 4)) == 0 ||
			(strncmp(buffer, "quit", 4)) == 0 ||
			(strncmp(buffer, "q", 1)) == 0)
		{
            printf("\nClient Exit...\n");
            break;
        }

		// get response from Server
        bzero(buffer, sizeof(buffer));
        read(sockfd, buffer, sizeof(buffer));
		printf("Response from Server : %s\n", buffer);
    }
}


