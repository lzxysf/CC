#include <stdio.h>
#include <unistd.h> //getpid
#include <stdlib.h>
#include <sys/time.h> //setitimer
#include <signal.h> //signal

/*
 *
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
当中which参数表示类型，可选的值有:
ITIMER_REAL:以系统真实时间来计算，它送出SIGALARM信号
ITIMER_VIRTUAL:以该进程在用户态下花费的时间来计算，它送出SIGVTALARM信号
ITIMER_PROF:以该进程在用户态和内核态下所费的时间来计算，它送出SIGPROF信号

itimerval结构体定义
struct itimerval {
	struct timeval it_interval;
	struct timeval it_value;
}
struct timeval {
	time_t tv_sec;
	suseconds_t tv_usec;
}

it_value为延时时长，it_interval为计时间隔。在运行过程中是延时it_value的时长后触发一次信号，之后每隔it_interval时长出发一次信号。
如果it_value为0是不会出发信号的，所以要能触发信号，it_value要大于0；假设it_interval为0，仅仅会延时，不会定时，也就是说只会触发一次信号。

tv_sec为秒，tv_usec为微秒（即1/1000000秒）

setitimer函数中的new_value参数用来对计时器进行设置，old_value通常用不上，设置为NULL,它是用来存储上一次setitimer调用时设置的new_value值。
 * 
 */

/*
本例setitimer设置定时，1s后触发SIGALRM事件，之后每隔3s触发一次SIGALARM事件。
事件被捕获处理。
*/

void myfunc(int signo)
{
	printf("hello world\r\n");
}

int main()
{
	struct itimerval it, oldit;
	signal(SIGALRM, myfunc);

	it.it_value.tv_sec = 1;
	it.it_value.tv_usec = 0;
	it.it_interval.tv_sec = 3;
	it.it_interval.tv_usec = 0;

	printf("当前进程pid=%d\r\n", getpid());

	if(setitimer(ITIMER_REAL, &it, &oldit)==-1)
	{
		perror("setitimer");
		exit(1);
	}

	while(1);
	
	return 0;
}
