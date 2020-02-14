#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/*
signal的作用是注册信号的捕捉事件。

sighandler_t signal(int signum, sighandler_t handler); 成功：返回函数指针；失败：返回SIG_ERR，设置errno

其中signum是信号值，handler是信号对应的捕捉函数，sighandler_t对应的函数原型为void (*sighandler_t)(int)

该函数由ANSI定义，由于历史原因在不同版本的Unix和不同版本的Linux中可能有不同的行为。因此应该尽量避免使用它，取而代之使用sigaction函数。
*/

void alarm_handler(int signo)
{
    printf("接收到了SIGALRM信号\r\n");
    exit(0);
}

int main()
{
    signal(SIGALRM, alarm_handler);

    alarm(3);

    while(1);

    return 0;
}
