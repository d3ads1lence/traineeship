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
#include "data.h"

#define MY_MQ_NAME "/my_mq"

static struct mq_attr my_mq_attr;
static mqd_t my_mq;
static int file_int_desc = 0;
static char buf[64];


int main(void)
{
	message_t msg;
	struct tm *sTm;
	
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

	FILE *file_int = fopen("int.txt", "a");
	FILE *file_array = fopen("array.txt", "a");
	FILE *file_struct = fopen("struct.txt", "a");	

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