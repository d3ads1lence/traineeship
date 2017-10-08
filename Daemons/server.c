#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"

int main(void)
{
	int msqid;
	int msgflg = IPC_CREAT | 0666;
	message_t msg;
	size_t ms_length;

	printf("Calling msgget with key %d\n", key);

	if ((msqid = msgget(key, msgflg)) < 0) {
		perror("msgget");
		exit(1);
	} else {
		printf("msgget: msgget succeeded: msqid = %d\n", msqid);	
	}

	// msg.ms_type = DIGIT;
	// msg.ms_body.dig = 777;
	// ms_length = sizeof(msg);

	// if (msgsnd(msqid, &msg, ms_length, IPC_NOWAIT) < 0) {
	// 	perror("msgsnd");
	// 	exit(1);
	// } else {
	// 	printf("Message sent, type: %d\n", msg.ms_type);
	// }

	msg.ms_type = ARRAY;
	(void) strcpy(msg.ms_body.arr, "test");
	ms_length = sizeof(msg);

	if (msgsnd(msqid, &msg, ms_length, IPC_NOWAIT) < 0) {
		perror("msgsnd");
		exit(1);
	} else {
		printf("Message sent, type: %d\n", msg.ms_type);
	}

	exit(0);
}
