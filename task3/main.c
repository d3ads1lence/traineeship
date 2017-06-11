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
#include <string.h>
#include <pthread.h>

sem_t *sem;
key_t key = 1000;
int shmid;
char SEM_NAME[]= "test";
void pipe_writer (FILE* stream);
void pipe_reader(FILE* stream);
void *Thread1(void *arg);
void *Thread2(void *arg);

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
			/********** Process C **********/
			pthread_t thread_c_1,thread_c_2;
			int number = 0;

			//create two threads to work on these semaphores
			if(pthread_create(&thread_c_1, NULL,Thread1, (void* )&number))
			{
				printf("\n ERROR creating thread 1");
				exit(1);
			}
			if(pthread_create(&thread_c_2, NULL,Thread2, (void* )&number))
			{
				printf("\n ERROR creating thread 2");
				exit(1);
			}
			//waiting on these threads to complete
			// pthread_join(thread_c_1, NULL);
			// pthread_join(thread_c_2, NULL);


			while(1){
			sleep(11);
			/********** Process C **********/
			}
		} else {
		/********** Process B **********/

		FILE *stream;
		close(fds[1]);
		stream = fdopen(fds[0], "r");
		pipe_reader(stream);
		close(fds[0]);
		/********** Process B **********/
		}
	} else {
		/********** Process A **********/
		FILE* stream;
		close(fds[0]);
		stream = fdopen(fds[1], "w");
		printf("Enter the number, please:\n");
		pipe_writer(stream);
		close(fds[1]);
		/********** Process A **********/
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
			printf("It's not a number, try again:\n");
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
	int* shared_memory_b = shmat(shmid,NULL,0);

	while (!ferror(stream) && fgets(buffer, sizeof (buffer), stream) != NULL) {
		int number = atoi(buffer);
		number *= number;
		memcpy(shared_memory_b, &number, sizeof(number));
		sem_post(sem);
	}
}


void *Thread1(void *arg)
{
	int *readed_number;
	readed_number = (int*)arg;
	//create the shared memory segment with this key
	shmid = shmget(key,getpagesize(),0666);
	if(shmid<0)
	{
		perror("reader:failure in shmget");
		exit(-1);
	}

	//attach this segment to virtual memory
	int* shared_memory_c = shmat(shmid,NULL,0);

	while(1) {
			if (sem_wait(sem) == 0){	
			memcpy(readed_number, shared_memory_c, sizeof(int));
			printf("Readed from shared memory number: %d\n", *readed_number);
		}
	}
}

void *Thread2(void *arg)
{
	int *number = 0;
	number = (int*)arg;
	while(1) {
		if (*number) {
			printf("value = %d\n", *number);
			*number = 0;
		} else {
			printf("I am alive\n");
		}
		sleep(1);
	}
}

