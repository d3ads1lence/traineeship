#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "settings.h"

int main (void)
{
	int socket_fd, client_socket_fd, client_adr_len, read_size;
	struct sockaddr_in server, client;
	char client_message[2000];

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) 
	perror("ERROR opening socket");
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    
    if( bind(socket_fd,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("ERROR bind failed");
        return 1;
    }
    listen(socket_fd, 128);
    printf("Wainting connections.\n");

    client_adr_len = sizeof(struct sockaddr_in);

	client_socket_fd = accept(socket_fd, (struct sockaddr *)&client, 
										(socklen_t*)&client_adr_len);
	if (client_socket_fd < 0){
		perror("ERROR accept failed");
        return 1;
	} else {
		printf("Client connected\n");
	}

    while(1)
    {
    	if(read_size = recv(client_socket_fd , client_message , 2000 , 0) > 0){
    		printf("Received message: ");
    		printf("%s", client_message);
    		memset(client_message, 0x00, 2000);
    	}
    	if(read_size == 0){
			printf("Client disconnected\n");
			break;
		}else if(read_size == -1){
			perror("ERROR recv failed");
		}
    }
    return 0;
}

