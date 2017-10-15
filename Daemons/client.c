#include <sys/types.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "data.h"

#define MY_MQ_NAME "/my_mq"

static struct mq_attr my_mq_attr;
static mqd_t my_mq;
static char file_path [256];
static char buf[256];
static char buf2[256];

int main(int argc, char *argv[])
{
    int mode = 0;
    int c = 0;
    int read_file = 0;
    int temp;

    message_t msg;
	
    my_mq_attr.mq_maxmsg = 10;
    my_mq_attr.mq_msgsize = sizeof(msg);

    while ((c = getopt(argc, argv, "f:")) != EOF) {
        switch (c){
            case 'f':
                strcpy(file_path, optarg);
                read_file = 1;
            break;
            case '?':
                printf("Usage ./client -f /path_to_file\n");
            exit(1);
        }
    }

    my_mq = mq_open(MY_MQ_NAME, O_WRONLY | O_NONBLOCK, \
                    0666, \
                    &my_mq_attr);
    if (my_mq < 0){
    	perror("mq_open");
		exit(1);
    }

    if (read_file){
        FILE *file_desc = fopen(file_path, "r");
        while (fgets(buf, 64, file_desc) != NULL){
            char *delimiter = strchr(buf, ':');
            int header_size = (delimiter - buf);
            strncpy(buf2, buf, header_size);
            int msg_type = atoi(buf2);
            switch (msg_type){
                    case DIGIT:
                        msg.ms_type = DIGIT;
                        sscanf(buf, "%i:%i", &temp, &msg.ms_body.dig);
                    break;
                    case ARRAY:
                        msg.ms_type = ARRAY;
                        sscanf(buf,"%i:%5s", &temp, msg.ms_body.arr);
                    break;
                    case STRUCT:
                        msg.ms_type = STRUCT;
                        sscanf(buf,"%i:%i,%i,%i", &temp, &msg.ms_body.my_struct.a, &msg.ms_body.my_struct.b, &msg.ms_body.my_struct.c);
                    break;
                    default:
                    printf("Unknown data type\n");
                    break; 
            }  
            if (mq_send(my_mq, (const char*)&msg, sizeof(msg), 1) < 0) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
        }

    } else {
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
                        printf("Wrong mode, choose 1 - int, 2 - char array or 3 - structure\n");
                    break;
                }
                if (mq_send(my_mq, (const char*)&msg, sizeof(msg), 1) < 0) {
                    perror("msgsnd");
                    exit(EXIT_FAILURE);
                } else {
                    printf("Select data type: ");
                }
            }
        }



	exit(0);
}
