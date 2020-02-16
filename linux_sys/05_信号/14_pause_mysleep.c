#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/**
 * pause函数是操作系统唯一一个主动造成进程挂起的系统调用，调用该系统调用的进程将处于阻塞状态直到有信号将其唤醒。（不区分特定的信号）
 *
 * int pause(void);
 *
 * 返回值：
 * (1)如果信号的默认处理动作是终止进程，则进程终止，pause函数没有机会返回。
 * (2)如果信号的默认处理动作是忽略，进程继续处于挂起状态，pause函数不返回。
 * (3)如果信号的处理动作是捕捉，则调用完信号处理函数之后，pause返回-1，errno设置为EINTR，表示“被信号中断”。
 * (4)pause收到的信号不能被屏蔽，如果被屏蔽，那么pause就不能被唤醒。
 *
 */

/*
 * 本例中使用pause和alarm来实现sleep函数 
 *
 */

void alarm_handler(int signo)
{
//do nothing
}

unsigned int mysleep(unsigned int sec)
{
	struct sigaction act, old_act;
	unsigned int unslept;
	
	act.sa_handler = alarm_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGALRM, &act, &old_act);

	alarm(sec);
	pause();

	unslept = alarm(0);//取消定时。由于pause可能在接收到SIGALRM信号之前接收到了其它信号，所以此处额外设置alarm(0)取消闹钟
	sigaction(SIGALRM, &old_act, NULL);//将原来的sigaction设置回去

	return unslept;//返回还未睡够的时间
}

int main()
{
	mysleep(3);
	printf("运行结束\r\n");
	return 0;
}

