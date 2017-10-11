#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "data.h"


int main(void)
{
    int msqid;
    int msgflg = IPC_CREAT | 0666;

    message_t msg;

    printf("Calling msgget with key %d\n", key);

	if ((msqid = msgget(key, msgflg)) < 0) {
		perror("msgget");
		exit(1);
	} else {
		printf("msgget: msgget succeeded: msqid = %d\n", msqid);	
	}

	while(1){
	    if (msgrcv(msqid, (void *) &msg, sizeof(msg.ms_body), 0, 
	    									MSG_NOERROR | IPC_NOWAIT) < 0) {
			if (errno != ENOMSG) {
			       perror("msgrcv");
			       exit(EXIT_FAILURE);
			   }
		} else {
			switch(msg.ms_type){
					case DIGIT:
						printf("Received digit: %i\n", msg.ms_body.dig);
					break;
					case ARRAY:
						printf("Received char array: %s\n", msg.ms_body.arr);
					break;
					case STRUCT:
						printf("Received struct: a: %i, b: %i, c: %i", 
												msg.ms_body.my_struct.a, 
												msg.ms_body.my_struct.b, 
												msg.ms_body.my_struct.c);
					break;
					default:
						printf("Unknown data type.");
					break;
			}
		}
	}


    exit(0);
}