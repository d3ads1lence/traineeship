#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
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
	    if (msgrcv(msqid, &msg, sizeof(message_t), 0, 0) < 0) {
		    perror("msgrcv");
		    exit(1);
		} else {
		    if (msg.ms_type == DIGIT){
		        printf("Received digit: %d\n", msg.ms_body.dig);
		    }
		    if (msg.ms_type == ARRAY){
		        printf("Received digit: %s\n", msg.ms_body.arr);
		    }
		}
	}


    exit(0);
}