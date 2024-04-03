#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	int fd;
	char buffer[6] = {"Hello!"};
	
	if (mkfifo("./pipe", 0777) == -1) {
		perror("mkfifo");
		exit(EXIT_FAILURE);
	}	
	
	fd = open("./pipe", O_WRONLY);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	
	write(fd, buffer, 6);
	close(fd);
	exit(EXIT_SUCCESS);
}

