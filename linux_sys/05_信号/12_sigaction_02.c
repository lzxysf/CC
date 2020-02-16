#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/*
sigaction函数的功能是检查或修改与指定信号相关联的处理动作

ing sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)

signum参数指定要捕获的信号类型，act参数指定新的信号处理方式，oldact参数输出先前的信号处理方式（如果不为NULL的话）
*/

/*
 *
 struct sigaction结构体介绍

 struct sigaction {
 	void (*sa_handler)(int);
	void (*sa_sigaction)(int, siginfo_t*, void*);
	sigset_t sa_mask;
	int sa_flags;
	void (*sa_restorer)(void)
 }

sa_handler:参数和signal()函数的参数handler相同，代表新的信号处理函数，也可以赋值为SIG_IGN表示忽略，或者赋值为SIG_DFL表示执行默认动作。
sa_mask:该信号处理函数执行过程时，所要屏蔽的信号集合（信号屏蔽字）注意：仅在处理函数被调用期间屏蔽生效，是临时性设置。
sa_flags:通常设置为0，表示使用默认属性。

 */

/*
 信号的捕捉特性:
 1.进程正常运行时，默认PCB中有一个信号屏蔽字，假定为☆，它决定了进程自动屏蔽哪些信号。
 当注册了某个信号捕捉函数，捕捉到该信号以后，要调用该函数，而该函数有可能执行很长时间，在这期间所屏蔽的信号不由☆来决定，而是用sa_mask来决定，调用完信号处理函数，再恢复为☆。
 2、xxx信号捕捉函数执行期间，xxx信号自动被屏蔽。
 3、阻塞的常规信号不支持排队，产生多次只记录一次。（后32个实时信号支持排队）
 */

/*
 *验证信号捕捉函数执行期间，该信号多次递送的效果:
 此时，该信号自动被屏蔽
 */

void alarm_handler(int signo)
{
    printf("接收到了SIGALRM信号\r\n");

    //延长信号捕捉函数执行时间
    sleep(5);
}

int main()
{
    struct sigaction act, oldact;
    act.sa_handler = alarm_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, &oldact);

    //发视多次SIGALRM信号
    alarm(3);
    alarm(4);
    alarm(5);

    while(1);

    return 0;
}
