#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	int fd;
	char buffer[6] = { 0 };	
	
	fd = open("./pipe", O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	
	read(fd, buffer, 6);
	printf("%s\n", buffer);
	close(fd);
	exit(EXIT_SUCCESS);
}
