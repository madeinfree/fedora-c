#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX 1
#define PORT 8080
#define SA struct sockaddr

int main() {
	int sockfd, connfd,  len;	
	struct sockaddr_in servaddr, cli;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed..\n");
		exit(0);
	} else {
		printf("socket successfully created..\n");
		bzero(&servaddr, sizeof(servaddr));

		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(PORT);

		if (bind(sockfd, (SA*)&servaddr,  sizeof(servaddr)) != 0) {
			printf("socket bind failed..\n");
			exit(0);
		}

		if (listen(sockfd, 5) != 0) {
			printf("listen failed..\n");	
			exit(0);
		} else {
			printf("Server listening..\n");
			// while(1) {
				len = sizeof(cli);
				connfd = accept(sockfd, (SA*)&cli, &len);

				if (connfd < 0) {
					printf("server accept failed..\n");
					exit(0);
				} else {
					printf("server accept the client..\n");
					char buff[10];
					int n;
					bzero(buff, sizeof(buff));
					while(1) {
						n = read(connfd, buff, sizeof(buff));
						if (n > 0) {
							if(buff[0] == 'e') break;
							printf("%s\n", buff);
							bzero(buff, sizeof(buff));
						}
					}
					close(sockfd);
					close(connfd);
					printf("Connection Over");
				}
			// }
		}
	}
}
