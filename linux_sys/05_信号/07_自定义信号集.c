#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/*
 *

阻塞信号集(信号屏蔽字)：
	将某些信号加入集合，对他们设置屏蔽，当屏蔽x信号后，再收到该信号，该信号的处理将推后(解除屏蔽后)。

未决信号集:
	1. 信号产生，未决信号集中描述该信号的位立刻翻转为1，表信号处于未决状态。当信号被处理对应位翻转回为0。这一时刻往往非常短暂。 
	2. 信号产生后由于某些原因(主要是阻塞)不能抵达。这类信号的集合称之为未决信号集。在屏蔽解除前，信号一直处于未决状态。
 
在PCB中有两个非常重要的信号集。一个称之为“阻塞信号集”，另一个称之为“未决信号集”。
这两个信号集都是内核使用位图机制来实现的。但操作系统不允许我们直接对其进行位操作。
而需自定义另外一个集合，借助信号集操作函数来对PCB中的这两个信号集进行修改。
 
自定义信号集设定
int sigemptyset(sigset_t *set);						将某个信号集清0				成功:0;失败:-1,设置errno
int sigfillset(sigset_t *set);						将某个信号集置1				成功:0;失败:-1,设置errno
int sigaddset(sigset_t *set, int signum);			将某个信号加入信号集合中	成功:0;失败:-1,设置errno
int sigdelset(sigset_t *set, int signum);			将某个信号清出信号集		成功:0;失败:-1,设置errno
int sigismember(const sigset_t *set, int signum);	判断某个信号是否在信号集中	在:1;不在:0;出错:-1,设置errno

除了sigismember外，其余操作函数中的set均为传出参数。sigset_t类型的本质是位图。但不应该直接使用位操作，而应该使用上述函数，保证跨系统操作有效。

 * 
 */


int main()
{
	sigset_t myset;
	sigemptyset(&myset);
	sigaddset(&myset, SIGINT);
	if(sigismember(&myset, SIGINT)==1)
	{
		printf("SIGINT在信号集中\r\n");
	}
	else
	{
		printf("SIGINT不在信号集中\r\n");
	}
	return 0;
}
