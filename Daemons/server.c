#include <sys/types.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "data.h"

#define MY_MQ_NAME "/my_mq"

static struct mq_attr my_mq_attr;
static mqd_t my_mq;


int main(void)
{
	message_t msg;
	
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

	while(1){
	    if (mq_receive(my_mq, (char*)&msg, sizeof(msg), NULL) > 0){
			switch(msg.ms_type){
					case DIGIT:
						printf("Received digit: %i\n", msg.ms_body.dig);
					break;
					case ARRAY:
						printf("Received char array: %s\n", msg.ms_body.arr);
					break;
					case STRUCT:
						printf("Received struct a: %i", msg.ms_body.my_struct.a);
						printf(" b: %i", msg.ms_body.my_struct.b);
						printf(" c: %i\n", msg.ms_body.my_struct.c);
					break;
					default:
						printf("Unknown data type.");
					break;
			}
		}
	}


    exit(0);
}