/*
进程组，也称之为作业。BSD于1980年前后向Unix中增加的一个新特性。代表一个或多个进程的集合。
每个进程都属于一个进程组，在waitpid函数和kill函数的参数中都曾使用到。
操作系统设计的进程组的概念，是为了简化对多个进程的管理。
当父进程，创建子进程的时候，默认子进程与父进程属于同一进程组。进程组ID==第一个进程ID(组长进程)。 
可以使用kill -SIGKILL -进程组ID(负的)来将整个进程组内的进程全部杀死。
只要进程组中有一个进程存在，进程组就存在，与组长进程是否终止无关。
进程组生存期：进程组创建到最后一个进程离开(终止或转移到另一个进程组)。
*/

/*
pid_t getpgrp(void);
获取当前进程的进程组ID
*/

#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t pid = getpid();
	printf("当前进程id为%d\r\n", pid);

	pid_t pgrp = getpgrp();
	printf("当前进程组id为%d\r\n", pgrp);

	return 0;
}
