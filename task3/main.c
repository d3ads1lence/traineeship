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
#include <sys/shm.h>
#include <sys/stat.h>

sem_t *sem;
key_t key = 1000;
int shmid;
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
			/* Process C */
			 //create the shared memory segment with this key
			shmid = shmget(key,getpagesize(),0666);
			if(shmid<0)
			{
				perror("reader:failure in shmget");
				exit(-1);
			}

			//attach this segment to virtual memory
			char* shared_memory_c = shmat(shmid,NULL,0);
			while(1){
				if (sem_wait(sem) == 0){
					printf("%s\n", shared_memory_c);
				}
			/* Process C */
			}
		} else {
		/* Process B */

		FILE *stream;
		close(fds[1]);
		stream = fdopen(fds[0], "r");
		pipe_reader(stream);
		close(fds[0]);
		/* Process B */
		}
	} else {
		/* Process A */
		FILE* stream;
		close(fds[0]);
		stream = fdopen(fds[1], "w");
		pipe_writer(stream);
		close(fds[1]);
		/* Process A */
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
	//create the shared memory segment with this key
	shmid = shmget(key,getpagesize(),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("failure in shmget");
		exit(-1);
	}
	char* shared_memory_b = shmat(shmid,NULL,0);

	while (!ferror(stream) && fgets(buffer, sizeof (buffer), stream) != NULL) {
		int number = atoi(buffer);
		if (number > 100) {
			sprintf(shared_memory_b, ">100");
			sem_post(sem);
		} else {
			printf("< 100\n");
		}
	}
}
