#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	pid_t pid;

	pid = fork();

	if(pid < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid > 0)
	{
		printf("parent process pid = %d\r\n", getpid());
		printf("parent group id = %d\r\n", getpgrp());	
		sleep(2);
	}
	else if(pid == 0)
	{
		printf("child process pid = %d\r\n", getpid());
		printf("child group id = %d\r\n", getpgrp());
		printf("child group id = %d\r\n", getpgid(0));
		printf("child group id = %d\r\n", getpgid(getpid()));
	}
	return 0;
}
