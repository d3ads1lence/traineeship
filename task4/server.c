#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "settings.h"

static void child_process(int sock);
static void *child_thread(void *arg);
static void exec_shell(int sock);

typedef enum {
	SERVER_NOT_DEFINED,
	SERVER_A,
	SERVER_B,
}server_t;

int main (int argc, char *argv[])
{
	int socket_fd, client_socket_fd, client_adr_len;
	struct sockaddr_in server, client;
	server_t type_of_server;
	pid_t pid;

	if (argc == 2){
		if (strcmp(argv[1], "-process") == 0) {
			type_of_server = SERVER_A;
		} else if (strcmp(argv[1], "-pthreads") == 0) {
			type_of_server = SERVER_B;
		} else {
			printf("You can only use -process or -pthreads flags\n");
			exit(1);
		}
	} else {
		printf("Invalid number of arguments\n");
		exit(1);
	}

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) 
		perror("ERROR opening socket");
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    
    if(bind(socket_fd,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
		perror("ERROR bind failed");
		exit(1);
    }
    listen(socket_fd, 128);
    printf("Wainting connections.\n");

    client_adr_len = sizeof(struct sockaddr_in);

    while(1) {
    	client_socket_fd = accept(socket_fd, (struct sockaddr *)&client, 
											(socklen_t*)&client_adr_len);
		if (client_socket_fd < 0){
			perror("ERROR accept failed");
			exit(1);
		} else {
			printf("Client connected\n");
		}
		if (type_of_server == SERVER_A) {
			pid = fork();
			if (pid == -1) {
				perror("ERROR can't create child process\n");
				exit(1);
			} else if (pid == 0) {
				close(socket_fd);
				child_process(client_socket_fd);
				exit(0);
			} else {
				close(client_socket_fd);
			}
		} else if (type_of_server == SERVER_B) {
			pthread_t thread_client;
			if(pthread_create(&thread_client, NULL, child_thread, 
											(void* )&client_socket_fd)) {
				perror("ERROR can't create thread\n");
			}
		} else {
			printf ("Unknown type of server\n");
			exit(1);
		}
    }
    return 0;
}

static void child_process(int sock) 
{
	exec_shell(sock);
}

static void *child_thread(void *arg)
{
	int sock = *(int *)arg;
	exec_shell(sock);
}

static void exec_shell(int sock) 
{
	dup2(sock, 0);
	dup2(sock, 1);
	dup2(sock, 2);

	execl("/bin/sh", "sh", NULL);
}
