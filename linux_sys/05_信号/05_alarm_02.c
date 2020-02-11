#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/*
设置定时器(闹钟)。在指定seconds后，内核会给当前进程发送14）SIGALARM信号。进程收到该信号，默认动作终止。
每个进程都有且只有唯一的一个定时器。

unsigned int alarm(unsigned int seconds); 返回0或剩余的秒数，无失败。

常用：取消定时器alarm(0)，返回旧闹钟余下秒数。

例：alarm(5) → 3sec → alarm(4) → 5sec → alarm(5) → alarm(0)
定时，与进程状态无关(自然定时法)！就绪、运行、挂起(阻塞、暂停)、终止、僵尸...无论进程处于何种状态，alarm都计时。
*/

int main()
{
    printf("当前进程pid=%d\r\n", getpid());
    alarm(5); //定时5s
    sleep(1); //休眠1s
    int n = alarm(0); //取消定时，并返回剩余定时时间。
    printf("剩余定时时间为%d\r\n", n);
    while(1);
    return 0;
}
