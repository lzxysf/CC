#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/*
 *
int sigpending(sigset_t *set)
读取当前进程的未决信号集
set传出参数,返回值：成功：0；失败：-1，设置errno

本例中由于在阻塞信号集中设置了SIGINT信号阻塞，因此在按下ctrl+c后未决信号集中SIGINT信号由于被阻塞信号集阻塞，SIGINT信号会一直保留在未决信号集

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
	sigset_t myset, oldset, curset;
	sigemptyset(&myset);
	sigaddset(&myset, SIGINT);
	
	sigprocmask(SIG_BLOCK, &myset, &oldset);

	while(1)
	{
		sigpending(&curset);
		printmask(&curset);
		sleep(1);
	}

	return 0;
}
