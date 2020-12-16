#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>

#define MAX 1
#define PORT 8080
#define SA struct sockaddr

int main() {
	int sockfd, connfd, len;	
	int max_fd;
	struct sockaddr_in servaddr;
	fd_set fds;
	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	char buff[10] = {0};
	int addr_len = sizeof(struct sockaddr_in);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed..\n");
		exit(-1);
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
			
		max_fd = sockfd;
		FD_ZERO(&fds);
		FD_SET(sockfd, &fds);		
		while(1) {	
			fd_set read_fds;
			read_fds = fds;
			switch(select(max_fd + 1, &read_fds, NULL, NULL, &timeout)) {
				case -1: exit(-1); break;
				case 0: {
					printf("timeout run");
					timeout.tv_sec = 10;
					break;
				}
				default:
					for(int i = 0; i < FD_SETSIZE; i++) {
						if(FD_ISSET(i, &read_fds)) {
							if(i == sockfd) {
								struct sockaddr_in cli;
								int new_fd;
								int len = sizeof(cli);
								new_fd = accept(sockfd, (struct sockaddr *)&cli, &len);

								printf("Accept client come from [%s:%u] by fd [%d]\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port), new_fd);

								FD_SET(new_fd, &fds);
								if(new_fd > max_fd)
									max_fd = new_fd;
							} else {
								int recv_len;
								bzero(buff, sizeof(buff));
								recv_len = recv(i, buff, sizeof(buff), 0);
								printf("Receive: len=[%d], msg=[%s]\n", recv_len, buff);
								if (recv_len == 0) {
									close(i);
									FD_CLR(i, &fds);
								}
							}
						}
					}
			}
			printf("Run front\n");
		}
		}
	}
}
