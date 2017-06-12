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
#include <signal.h>

sem_t *sem;
key_t key = 1000;
int shmid;
char SEM_NAME[]= "msg_sem";
pthread_t thread_c_1,thread_c_2;
sig_atomic_t termination;
pid_t pid_C;
void *Thread_C1(void *arg);
void *Thread_C2(void *arg);
void handler(int signal_number);

int main() {
	int fds[2];
	pid_t pid, pid2;
	//Creating a pipe
    int result = pipe (fds);
    if (result < 0) {
        //failure in creating a pipe
        perror("pipe");
        exit (1);
    }
  	
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
			pid_C = getpid();
			int number = 0;
			//create two threads to work on these semaphores
			if(pthread_create(&thread_c_1, NULL,Thread_C1, (void* )&number))
			{
				perror("ERROR creating thread 1");
				exit(1);
			}
			if(pthread_create(&thread_c_2, NULL,Thread_C2, (void* )&number))
			{
				perror("ERROR creating thread 2");
				exit(1);
			}
			//waiting on these threads to complete
			pthread_join(thread_c_1, NULL);
			pthread_join(thread_c_2, NULL);
			while(1){
			sleep(1);
			/********** Process C **********/
			}
		} else {
		/********** Process B **********/
		FILE *stream;
		struct sigaction sa;
		memset(&sa, 0, sizeof(sa));
 		sa.sa_handler = &handler;
 		sigaction(SIGUSR1, &sa, NULL);
		close(fds[1]);
		char buffer[10];	
		//create the shared memory segment with this key
		shmid = shmget(key,getpagesize(),IPC_CREAT|0666);
		if(shmid<0)
		{
			perror("failure in shmget");
			exit(-1);
		}
		int* shared_memory_b = shmat(shmid,NULL,0);
		int number = 0;
		while(1) {
			read(fds[0], &number, sizeof(int));
			number *= number;
			memcpy(shared_memory_b, &number, sizeof(number));
			sem_post(sem);
			if (termination) {
				sem_close(sem);
				sem_unlink(SEM_NAME);
				shmctl(shmid, IPC_RMID, NULL);
				pthread_cancel(thread_c_1);
				pthread_cancel(thread_c_2);
				kill(pid_C, SIGTERM);
				kill(getppid(), SIGTERM);
				kill(getpid(), SIGTERM);
			}
		}
		exit (0);
		/********** Process B **********/
		}
	} else {
		/********** Process A **********/
		char line[10];
		int number = 0;

		close(fds[0]);
		printf("Enter the number, please:\n");
		while (fgets(line, sizeof(line), stdin) != 0) {
			if (sscanf(line, "%d", &number) == 1) {
				write(fds[1], &number, sizeof(int));
			} else {
				printf("It's not a number, try again:\n");
			}
		}
		exit (0);
		/********** Process A **********/
	}
return 0;
}


void *Thread_C1(void *arg)
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
			if (*readed_number > 100) 
				kill(getppid(), SIGUSR1);
		}
	}
}

void *Thread_C2(void *arg)
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

void handler(int signal_number) 
{
	termination = 1;
}