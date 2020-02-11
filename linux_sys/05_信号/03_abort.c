#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
abort 函数：给自己发送异常终止信号 6) SIGABRT 信号，终止并产生core文件
void abort(void); 该函数无返回
*/

int main()
{
    printf("当前进程pid=%d\r\n", getpid());
    abort();
    return 0;
}
