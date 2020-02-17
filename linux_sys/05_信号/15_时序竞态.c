#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/*
 * 使用pause和alarm来实现sleep函数
 *
 *
 * 但是这里有一个问题，假设定时3s。如果在“alarm(sec)”设置定时和“pause()挂起进程”之间由于时间片被用完，导致该进程被调出（这时alarm函数已经被执行，内核已经开始计时），而且在该系统的进程数很多，导致“饥饿现象”，所以该进程很久没有被调入，假设4s都没有被调入（我们设置的定时为3s），这时在内核中其实已经发出SIGALRM信号，并执行它的信号处理函数。此后进程获得了时间片并恢复运行，但是由于alarm发出的SIGALRM信号在之前已经被执行，所以pause()会永远等待不到SIGALRM信号的到达，那么该进程就永远被挂起！
  简单的说就是，信号抵达时，进程没有处于运行状态，导致进程继续运行后无法再收到这个信号了。
  再通俗点理解就是，有人到我家找我，但是我在睡觉，所以就接不到这个人了。 

 * 以上的情况就是一种由信号引起的竞态，这种情况是不可预知的，而且一旦出现后果十分严重。
 *
*/

/*
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
*/

/**
 *
 *如上情况可以用一个通俗的例子说明，假设如下场景:
  
 欲睡觉，定闹钟10分钟，希望10分钟后闹铃将自己唤醒。

 正常：定时，睡觉，10分钟后被闹钟唤醒。

 异常：闹钟定好后，被唤走，外出了，20分钟后从外边会来，回来继续睡觉计划，但外出期间闹钟已经到时响铃过了，不会再将我唤醒。
 *
 *
 * 可以通过设置SIGALRM的方法来控制程序执行逻辑，但无论如何设置，
 程序都有可能在“接触信号屏蔽与”挂起等待信号“这两个操作间隙失去cpu资源。
 除非将这两步骤合并为一个“原子操作”，sigsuspend函数具备这个功能。
 在对时序要求严格的场合下都应该使用sigsuspend替换pause。
 *
 *
 *
 * 要改变以上情况，可以使用sigsuspend函数。
 * 在alarm()执行前，设置SIGALRM信号阻塞，防止SIGALRM信号丢弃。
 * 执行到sigsuspend(&mask)时，设置阻塞信号集为mask,mask中不阻塞SIGALRM信号，此时就可以接收到SIGALRM信号。
 *
 *
 */

/*
竞态条件，跟系统负载有很紧密的关系，体现出信号的不可靠性。系统负载越严重，信号的不可靠性越强。
不可靠由其实现原理所致。信号是通过软件方法实现（跟内核调度高度依赖，延时性强）。当系统负载过重时，会出现时序混乱。
这种意外情况只能在编写程序过程中，提早预见，主动规避，而无法通过gdb程序调试等其它手段弥补。且由于该错误不具有规律性，
后期捕获和重现十分困难。
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int signo)
{
//do nothing
}

unsigned int mysleep(unsigned int sec)
{
	//设置SIGALRM信号的默认处理函数
	struct sigaction act, old_act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGALRM, &act, &old_act);
	
	//设置阻塞信号集，阻塞SIGALRM信号
	sigset_t set, old_set;
	sigemptyset(&set);
	sigaddset(&set, SIGALRM);
	sigprocmask(SIG_SETMASK, &set, &old_set);

	//设置定时发送SIGALRM信号
	alarm(sec);

	//设置sigsuspend执行期间的信号集
	sigset_t susp_set;
	susp_set = old_set;
	sigdelset(&susp_set, SIGALRM);

	//挂起
	sigsuspend(&susp_set);
	
	unsigned int uslept = alarm(0);//如果在正常的SIGALRM信号到来之前，接收到其它信号，则计算未睡够时间
	sigaction(SIGALRM, &old_act, NULL);
	sigprocmask(SIG_SETMASK, &old_set, NULL);

	return uslept;
}


int main()
{
	mysleep(3);

	printf("定时结束\r\n");

	return 0;
}
