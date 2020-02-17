/*
 *
 *SIGCHLD回收多个子进程

 由于多个子进程可能同时结束，多个SIGCHLD信号同时递达主进程，但是普通信号是不支持排队的，因此在SIGCHLD信号处理函数中要使用while和waitpid来进行多次回收，以回收所有终止了的子进程。
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void collect_child_handler(int signo)
{
	int status;
	pid_t pid;
	while((pid = waitpid(0, &status, WNOHANG))>0)	//0-等待同一个进程组中的任何子进程，WNOHANG-非阻塞，返回值>0-成功,返回回收的子进程pid
	{
		if(WIFEXITED(status))
		{
			printf("child %d exit %d\n", pid, WEXITSTATUS(status));
		}
		else if(WIFSIGNALED(status))
		{
			printf("child %d cancel signal %d\r\n", pid, WTERMSIG(status));
		}
		printf("回收子进程\r\n");
	}
}

int main()
{
	pid_t pid;

	for(int i = 0; i < 10; i++)
	{
		pid = fork();
		if(pid == 0)
		{
			break;
		}
		else if(pid < 0)
		{
			perror("fork error");
			exit(1);
		}
	}

	//父进程
	if(pid > 0)
	{
		//注册SIGCHLD信号的捕获函数
		struct sigaction act, old_act;
		act.sa_handler = collect_child_handler;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGCHLD, &act, &old_act);
	
		printf("父进程pid=%d\r\n", getpid());
		while(1);

	}
	//子进程
	else if(pid == 0)
	{
		printf("子进程pid=%d\r\n", getpid());
		sleep(1);
	}
	return 0;
}

