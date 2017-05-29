/* main.c */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void pipe_writer (FILE* stream);
void pipe_reader(FILE* stream);

int main() {
	int fds[2];
	pid_t pid;
	pipe(fds);
	
	pid = fork();
	if (pid == (pid_t)0) {
		FILE *stream;
		close(fds[1]);
		stream = fdopen(fds[0], "r");
		pipe_reader(stream);
		close(fds[0]);
	} else {
		FILE* stream;
		close(fds[0]);
		stream = fdopen(fds[1], "w");
		pipe_writer(stream);
		close(fds[1]);
	}
return 0;
}

void pipe_writer (FILE* stream)
{
    char line[10];
    int number = 0;

    while (fgets(line, sizeof(line), stdin) != 0)
    {
        if (sscanf(line, "%d", &number) == 1){
        	fprintf(stream, "%d\n", number);
			fflush(stream);
		} else {
			printf("-> Wrong format, try again! <-\n");
		}
    }
}

void pipe_reader(FILE* stream)
{
	char buffer[10];
	while (!ferror(stream) && fgets(buffer, sizeof (buffer), stream) != NULL) {
		printf("This number was readed from pipe: ");
		fputs(buffer, stdout);
		printf("\n");
	}
}
