#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
	int pipefd[2];
	pid_t child_pid;
	char buffer1[6] = {"Hello!"};
	char buffer2[6] = { 0 };
	
	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	
	child_pid = fork();
	if (child_pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	
	if (child_pid == 0) {
		close(pipefd[1]);
		read(pipefd[0], buffer2, 6);
		printf("%s\n", buffer2);
		close(pipefd[0]);
		exit(EXIT_SUCCESS);
	} else {
		close(pipefd[0]);
		write(pipefd[1], buffer1, 6);
		close(pipefd[1]);
		wait(NULL);
		exit(EXIT_SUCCESS);
	}
}
