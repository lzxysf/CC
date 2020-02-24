#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
pid_t pid = setsid(void);
功能为创建一个会话。

运行setsid()后,执行以下操作:
1)创建一个会话，SessionID就是当前进程的pid，当前进程成为会话的首进程
2)创建一个进程组，进程组ID就是当前进程的pid，当前进程成为进程组的组长进程

组长进程不能成为新会话的首进程，新会话的首进程必定成为组长进程

setsid()调用失败返回-1，调用成功返回新会话的ID

在组长进程中调用setsid()会失败，返回-1

在某个非组长进程中，执行setsid()成功后，当前进程会与其父进程的会话组与进程组脱离。由于会话对控制终端的独占性，进程同时与控制终端脱离。
*/

int main()
{
	pid_t pid;
	pid = fork();
	if(pid < 0)
	{
		perror("fork error");
		return 0;
	}
	else if(pid > 0)
	{
		wait(NULL);
	}
	else if(pid == 0)
	{
		printf("当前进程PID为%d\r\n", getpid());
		printf("当前进程进程组ID为%d\r\n", getpgid(0));
		printf("当前进程会话组ID为%d\r\n", getsid(0));
		
		sleep(1);
		printf("setsid后...\r\n");
		setsid();
		
		printf("当前进程PID为%d\r\n", getpid());
		printf("当前进程进程组ID为%d\r\n", getpgid(0));
		printf("当前进程会话组ID为%d\r\n", getsid(0));
	}
	return 0;
}
