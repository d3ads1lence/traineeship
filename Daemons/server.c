#include <sys/types.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <syslog.h>
#include <signal.h>
#include <sys/resource.h>
#include "data.h"


#define MY_MQ_NAME "/my_mq"

static void daemonize(void);


int main(int argc, char *argv[])
{
	int c = 0;
	int daemon = 0;
	struct mq_attr my_mq_attr;
 	mqd_t my_mq;
 	char buf[64];
 	char int_file_name [256] = "int.txt";
 	char array_file_name [256] = "array.txt";
 	char struct_file_name [256] = "struct.txt";
 	message_t msg;
 	struct tm *sTm;


	while ((c = getopt(argc, argv, "Di:a:s:")) != EOF) {
			switch (c) {
				case 'D': 
					daemon = 1;
				break;
				case 'i':
					strcpy(int_file_name, optarg);
				break;
				case 'a':
					strcpy(array_file_name, optarg);
				break;
				case 's':
					strcpy(struct_file_name, optarg);
				break;
				case '?':
					printf("Usage ./server -D for daemon, -i int.txt, -a arr.txt, -s struct.txt\n");
					exit(1);
			}
	}

	if (daemon){
		daemonize();
		syslog (LOG_NOTICE, "Daemon started.");
	}
	my_mq_attr.mq_maxmsg = 10;
    my_mq_attr.mq_msgsize = sizeof(msg);

	my_mq = mq_open(MY_MQ_NAME, \
					O_CREAT | O_RDONLY | O_NONBLOCK, \
					0666, \
					&my_mq_attr);
    if (my_mq < 0){
    	perror("mq_open");
		exit(1);
    }

	FILE *file_int = fopen(int_file_name, "a");
	FILE *file_array = fopen(array_file_name, "a");
	FILE *file_struct = fopen(struct_file_name, "a");	

	while(1){

	    if (mq_receive(my_mq, (char*)&msg, sizeof(msg), NULL) > 0){
	    	time_t now = time (0);
    		sTm = gmtime (&now);
    		strftime (buf, sizeof(buf), "%d-%m-%Y,%H.%M.%S", sTm);
			switch(msg.ms_type){
					case DIGIT:
						fprintf(file_int, "%s:%i\n", buf, msg.ms_body.dig);
						fflush(file_int);
					break;
					case ARRAY:
						fprintf(file_array, "%s:%s\n", buf, msg.ms_body.arr);
						fflush(file_array);
					break;
					case STRUCT:
						fprintf(file_struct, "%s:%i,%i,%i\n", buf, \
											msg.ms_body.my_struct.a, \
											msg.ms_body.my_struct.b, \
											msg.ms_body.my_struct.c);
						fflush(file_struct);
					break;
					default:
						printf("Unknown data type.");
					break;
			}
		}
	}

	fclose(file_int);
	fclose(file_array);
	fclose(file_struct);

    exit(0);
}

static void daemonize(void)
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }

    /* Open the log file */
    openlog ("my_daemon", LOG_PID, LOG_DAEMON);
}

