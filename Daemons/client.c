#include <sys/types.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "data.h"

#define MY_MQ_NAME "/my_mq"

static struct mq_attr my_mq_attr;
static mqd_t my_mq;

int main(int argc, char *argv[])
{
    int mode = 0;

    message_t msg;
	
    my_mq_attr.mq_maxmsg = 10;
    my_mq_attr.mq_msgsize = sizeof(msg);

    my_mq = mq_open(MY_MQ_NAME, O_WRONLY | O_NONBLOCK, \
                    0666, \
                    &my_mq_attr);
    if (my_mq < 0){
    	perror("mq_open");
		exit(1);
    }

    printf("Select data type: ");
    while (scanf("%d", &mode) == 1){
        switch (mode){
            case DIGIT:
                printf("Enter the integer variable\n");
                scanf("%i", &msg.ms_body.dig);
                msg.ms_type = DIGIT;
            break;
            case ARRAY:
                printf("Enter the char array\n");
                scanf("%5s", msg.ms_body.arr);
                msg.ms_type = ARRAY;
            break;
            case STRUCT:
                do{
                    printf("Enter 3 integer variables\n");
                }while(scanf("%i %i %i", &msg.ms_body.my_struct.a, &msg.ms_body.my_struct.b, &msg.ms_body.my_struct.c) !=3);
                msg.ms_type = STRUCT;
            break;
            default:
                printf("Wrong mode\n");
            break;
        }
        if (mq_send(my_mq, (const char*)&msg, sizeof(msg), 1) < 0) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        } else {
            printf("Select data type: ");
        }
    }

	exit(0);
}
