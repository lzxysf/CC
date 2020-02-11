#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/*
raise 函数：给当前进程发送指定信号(自己给自己发)	raise(signo) == kill(getpid(), signo);
int raise(int sig); 成功：0，失败非0值
*/

int main()
{
    printf("当前进程pid=%d\r\n", getpid());
    raise(SIGKILL);
    return 0;
}
