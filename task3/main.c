/* main.c */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

sem_t *sem;
char SEM_NAME[]= "test";
void pipe_writer (FILE* stream);
void pipe_reader(FILE* stream);

int main() {
	int fds[2];
	pid_t pid, pid2;
	pipe(fds);

	sem = sem_open(SEM_NAME,O_CREAT,0644,0);
	 if(sem == SEM_FAILED)
    {
		perror("unable to create semaphore");
		sem_unlink(SEM_NAME);
		exit(-1);
    }
	
	pid = fork();
	if (pid == (pid_t)0) {
		pid2 = fork();
		if (pid2 == 0){
			while(1){
				if (sem_wait(sem) == 0){
					printf("> 100\n");
				}
			}
		} else {
		FILE *stream;
		close(fds[1]);
		stream = fdopen(fds[0], "r");
		pipe_reader(stream);
		close(fds[0]);
		}
	} else {
		FILE* stream;
		close(fds[0]);
		stream = fdopen(fds[1], "w");
		pipe_writer(stream);
		close(fds[1]);
	}
return 0;
}

void pipe_writer (FILE* stream)
{
    char line[10];
    int number = 0;

    while (fgets(line, sizeof(line), stdin) != 0)
    {
        if (sscanf(line, "%d", &number) == 1){
        	fprintf(stream, "%d\n", number);
			fflush(stream);
		} else {
			printf("-> Wrong format, try again! <-\n");
		}
    }
}

void pipe_reader(FILE* stream)
{
	char buffer[10];
	while (!ferror(stream) && fgets(buffer, sizeof (buffer), stream) != NULL) {
		int number = atoi(buffer);
		if (number > 100) {
			// sem_wait(&sem);
			// printf("> 100\n");
			sem_post(sem);
		} else {
			printf("< 100\n");
		}
		// printf("This number was readed from pipe: %d\n", number);
		// fputs(buffer, stdout);
	}
}
