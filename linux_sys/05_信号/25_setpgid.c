#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
int setpgid(pid_t pid, pid_t pgid);
将pid进程加入pgid进程组。
改变进程默认所属的进程组,通常可以用来加入一个现有的进程组或创建一个新的进程组。

注意：权级问题。非root进程只能改变自己创建的子进程，或有权限操作的进程。
*/

int main()
{
	pid_t pid;
	pid = fork();
	if(pid > 0)
	{
		printf("parent pid = %d\r\n", getpid());
		sleep(1);
		setpgid(pid, pid);//让子进程自立门户，成为进程组组长，以它的pid为进程组id
		wait(NULL);
	}
	else if(pid == 0)
	{
		printf("child pid = %d\r\n", getpid());
		printf("child group id = %d\r\n", getpgid(0));
		sleep(2);
		printf("after 2 seconds...\r\n");
		printf("child pid = %d\r\n", getpid());
		printf("child group id = %d\r\n", getpgid(0));
	}
}
