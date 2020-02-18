#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/*
 *
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
sigprocmask函数是用来修改进程控制块中的信号屏蔽字,修改后的阻塞信号集=原来的阻塞信号集|set

严格注意：屏蔽信号，知识将信号处理延后执行（延后至解除屏蔽），而忽略表示将信号丢弃处理

how : 指示如何修改当前信号集
	SIG_BLOCK：当how设置为此值，set表示需要屏蔽的信号，相当于mask = mask | set
	SIG_UNBLOCK: 当how设置为此值，set表示需要解除屏蔽的信号，相当于mask = mask & ~set
	SIG_SETMASK：当how设置为此值，set表示用于替代原始屏蔽集的新的屏蔽集，相当于mask = set
set : 传入参数，是一个自定义信号集合
oldset ： 传出参数，保存旧的信号屏蔽字
 * 
 */

void printmask(sigset_t *set)
{
	for(int i = 1; i <= 31; i++)
	{
		if(sigismember(set, i)==1)
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
	}
	printf("\r\n");
}

int main()
{
	sigset_t myset, oldset;
	sigemptyset(&myset);
	sigaddset(&myset, SIGINT);
	
	sigprocmask(SIG_BLOCK, &myset, &oldset);

	//打印原来的信号屏蔽字
	printmask(&myset);

	return 0;
}
