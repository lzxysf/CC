/*
 *
 *SIGCHLD信号

  1、SIGCHLD的产生条件:
  1)子进程终止时;
  2)子进程接收到SIGSTOP信号停止时;
  3)子进程处于停止态，接收到SIGCOUT后唤醒。

  2、借助SIGCHLD信号回收子进程
    子进程结束运行，其父进程会收到SIGCHLD信号，该信号的默认处理动作是忽略。可以捕捉该信号，在捕捉函数中完成子进程状态的回收。
 */


/*
 * 本例为借助SIGCHLD信号回收子进程
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void collect_child_handler(int signo)
{
	wait(0);
	printf("回收子进程\r\n");
	printf("当前进程pid=%d\r\n", getpid());
}

int main()
{
	pid_t pid;

	pid = fork();
	if(pid < 0)
	{
		perror("fork error");
		exit(1);
	}
	//父进程
	else if(pid>0)
	{
		//注册SIGCHLD信号的捕获函数
		struct sigaction act, old_act;
		act.sa_handler = collect_child_handler;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGCHLD, &act, &old_act);
	
		while(1)
		{
			printf("父进程pid=%d\r\n", getpid());
			sleep(1);
		}

	}
	//子进程
	else if(pid == 0)
	{
		printf("子进程pid=%d\r\n", getpid());
		sleep(1);
	}
	return 0;
}

