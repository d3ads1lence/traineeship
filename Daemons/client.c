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
	message_t msg;
	size_t ms_length;

	printf("Calling msgget with key %d\n", key);

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    while (1){
        int digit = 0;
        scanf("%i", &digit);
        msg.ms_type = DIGIT;
        msg.ms_body.dig = digit;
        ms_length = sizeof(msg);

        if (msgsnd(msqid, &msg, ms_length, IPC_NOWAIT) < 0) {
         perror("msgsnd");
         exit(1);
        } else {
         printf("Message sent, type: %d\n", msg.ms_type);
        }
    }

	exit(0);
}
