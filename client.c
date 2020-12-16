#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX 10240
#define PORT 8080
#define SA struct sockaddr

int main() {
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed..\n");
		exit(0);
	}

	char buff[10];
	for(int i = 0; i < sizeof(buff); i++) {
		buff[i] = 'a';
	}
	write(sockfd, buff, sizeof(buff));
	bzero(buff, sizeof(buff));
	write(sockfd, "e", sizeof(char));
	close(sockfd);
}
