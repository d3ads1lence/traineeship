#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "settings.h"

int main (int argc, char* const argv[])
{
	int socket_fd;
	struct sockaddr_in client;
	char message[1000];
	
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1){
		perror("ERROR coldn't create socket\n");
	}
	
	client.sin_addr.s_addr = inet_addr("127.0.0.1");
	client.sin_family = AF_INET;
	client.sin_port = htons(PORT);
	
	if (connect(socket_fd, (struct sockaddr *)&client, sizeof(client)) < 0){
		perror("ERROR Connect filed\n");
		return 1;
	}
	
	printf("Connected.\n");
	
	while(1){
		memset(message, 0x00, 1000);
		while (1){
			printf("You: ");
			if (fgets(message, 1000, stdin) != NULL){
				if(send(socket_fd, message, strlen(message), 0) < 0){
            	perror("ERROR Send filed\n");
            	return 1;
        		}
			}
		}
	}
	close(socket_fd);
	return 0;
}