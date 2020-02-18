/*
 *
 *SIGCHLD回收多个子进程_进阶
 考虑子进程中无sleep()，即子进程创建后立即终止，此时主进程能捕获其SIGCHLD信号吗？
	有不能捕获的可能，因为此时主进程的SIGCHLD信号捕获函数还没有注册，SIGCHLD信号递达后执行信号默认处理动作（即忽略），而不是捕获SIGCHLD信号。
	解决的办法是在子进程创建之前（即fork函数执行之前）首先将SIGCHLD设置进阻塞信号集，待信号捕获函数注册后，再删除阻塞信号集的SIGCHLD。

SIGCHLD信号注意问题
	1.子进程继承了父进程的信号屏蔽字和信号处理动作，但子进程没有继承未决信号集spending。
	2.应该在fork之前，阻塞SIGCHLD信号，注册完信号捕捉函数后解除阻塞。
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
	sigset_t set, old_set;

	//设置SIGCHLD阻塞信号集
	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigprocmask(SIG_BLOCK, &set, &old_set);

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

		//解除SIGCHLD信号的阻塞
		sigdelset(&old_set, SIGCHLD);
		sigprocmask(SIG_SETMASK, &old_set, NULL);
	
		printf("父进程pid=%d\r\n", getpid());
		while(1);
	}
	//子进程
	else if(pid == 0)
	{
		printf("子进程pid=%d\r\n", getpid());
		// sleep(1);
	}
	return 0;
}
