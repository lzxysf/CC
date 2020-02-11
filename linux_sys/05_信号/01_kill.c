#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/*
子进程5s后kill父进程
*/

/*
    kill函数不是只能发送SIGKILL信号，它也可以发送其它信号。
    int kill(pid_t pid, int sig);	 成功：0；失败：-1 (ID非法，信号非法，普通用户杀init进程等权级问题)，设置errno
    sig：不推荐直接使用数字，应使用宏名，因为不同操作系统信号编号可能不同，但名称一致。
    pid > 0:  发送信号给指定的进程。
	pid = 0:  发送信号给与调用kill函数进程属于同一进程组的所有进程。
	pid < -1:  取|pid|发给对应进程组。
	pid = -1：发送给进程有权限发送的系统中所有进程。
*/

int main()
{
    pid_t pid = fork();
    if(pid > 0)
    {
        while(1)
        {
            printf("test\r\n");
            sleep(1);
        }
    }
    else
    {
        printf("父进程pid=%d,进程pid=%d\r\n", getppid(), getpid());
        sleep(5);
        kill(getppid(), SIGKILL);
    }
    return 0;
}
