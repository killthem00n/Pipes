#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void piped(char **command1, char **command2)
{
	int pipefd[2];
	pid_t child_pid1, child_pid2;
	int status1, status2;
	
	pipe(pipefd);
	
	child_pid1 = fork();
	if (child_pid1 == 0) {
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		execvp(command1[0], command1);
	} else if (child_pid1 > 0) {
		wait(&status1);
	} else {
		perror("fork1");
		exit(1);
	}
	
	child_pid2 = fork();
	if (child_pid2 == 0){
		close(pipefd[1]);
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		execvp(command2[0], command2);
	} else if (child_pid2 > 0) {
		wait(&status2);
	} else {
		perror("fork2");
		exit(1);
	}
}

int main(void)
{
	char input[1024];
	char *tokens[128];
	char *command1[64];
	char *command2[64];
	char *token;
	int numTokens = 0;
	int pipe_pos = 0;
	
	while (1) {
		printf("command: ");
		
		if ((fgets(input, sizeof(input), stdin)) == NULL) {
			perror("fgets");
			exit(1);
		}
		
		if ((strcmp(input, "exit\n")) == 0) {
			return 0;
		}
		
		input[strcspn(input, "\n")] = 0;	
		token = strtok(input, " ");
		
		while (token != NULL) {
			tokens[numTokens] = token;
			if ((strcmp(token, "|")) == 0 ) {
				pipe_pos = numTokens;
			}
			numTokens++;
			token = strtok(NULL, " ");
		}
		
		for (int i = 0; i < pipe_pos; i++) {
			command1[i] = tokens[i];
		}
		command1[pipe_pos] = NULL;

		for (int i = pipe_pos+1; i < numTokens; i++) {
			command2[i-pipe_pos-1] = tokens[i];
		}
		command2[numTokens-pipe_pos-1] = NULL;	
		
		piped(command1, command2);
	}
	
	return 0;
}
