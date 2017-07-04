#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include "settings.h"

#define BUFF_SIZE 4096

static int receive_all(int sock);

int main (int argc, char* const argv[])
{
	int socket_fd;
	struct sockaddr_in client;
	char message[BUFF_SIZE], received_msg[BUFF_SIZE];
	
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1){
		perror("ERROR coldn't create socket\n");
	}
	
	struct timeval tv;
	tv.tv_sec = 0;  
	tv.tv_usec = 500; 
	setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));

	client.sin_addr.s_addr = inet_addr("127.0.0.1");
	client.sin_family = AF_INET;
	client.sin_port = htons(PORT);
	
	if (connect(socket_fd, (struct sockaddr *)&client, sizeof(client)) < 0){
		perror("ERROR Connect filed\n");
		return 1;
	}
	
	printf("Connected.\n");
	while (1){
		printf("Enter the command: ");
		if (fgets(message, BUFF_SIZE, stdin) != NULL){
			if(send(socket_fd, message, strlen(message), 0) < 0){
			perror("ERROR Send filed\n");
			return 1;
			}
		}
	receive_all(socket_fd);
	}
	close(socket_fd);
	return 0;
}

static int receive_all(int sock)
{
int size_recv , total_size= 0;
char chunk[BUFF_SIZE];

while(1) {
	memset(chunk ,0 , BUFF_SIZE);  //clear the variable
	if((size_recv =  recv(sock , chunk , BUFF_SIZE , 0)) < 0) {
		break;
	} else {
		total_size += size_recv;
		printf("%s", chunk);
	}
}

return total_size;
}