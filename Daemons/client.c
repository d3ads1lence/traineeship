#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "data.h"

int main(void)
{
	int msqid = 0;
	message_t msg;
	size_t ms_length = 0;
    int mode = 0;

	printf("Calling msgget with key %d\n", key);

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    printf("Select data type: ");
    while (scanf("%d", &mode) == 1){
        switch (mode){
            case DIGIT:
            {
                printf("Enter the integer variable\n");
                scanf("%i", &msg.ms_body.dig);
                msg.ms_type = DIGIT;
                ms_length = sizeof(sizeof(msg.ms_body));
                if (msgsnd(msqid, (void *) &msg, ms_length, 0) < 0) {
                    perror("msgsnd");
                    exit(EXIT_FAILURE);
                } else {
                    printf("Select data type: ");
                }

            }
            break;
            case ARRAY:
            {
                printf("Enter the char array\n");
                scanf("%5s", msg.ms_body.arr);
                msg.ms_type = ARRAY;
                ms_length = sizeof(sizeof(msg.ms_body));
                if (msgsnd(msqid, (void *) &msg, ms_length, 0) < 0) {
                    perror("msgsnd");
                    exit(EXIT_FAILURE);
                } else {
                    printf("String: %s\n", msg.ms_body.arr);
                    printf("Select data type: ");
                }
            }
            break;
            case STRUCT:
            {
                printf("Enter 3 integer variables\n");
                scanf("%i %i %i", &msg.ms_body.my_struct.a, 
                        &msg.ms_body.my_struct.b, &msg.ms_body.my_struct.c);
                msg.ms_type = STRUCT;
                ms_length = sizeof(sizeof(msg.ms_body));
                if (msgsnd(msqid, (void *) &msg, ms_length, 0) < 0) {
                    perror("msgsnd");
                    exit(EXIT_FAILURE);
                } else {
                    printf("Select data type: ");
                }
            }
            break;
            default:
                printf("Wrong mode\n");
            break;
        }
    }

	exit(0);
}
