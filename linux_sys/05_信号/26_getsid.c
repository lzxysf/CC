#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
pid_t getsid(pid_t pid);成功：返回调用进程的会话ID；失败：-1，设置errno
获取进程所属的会话ID
pid为0表示获取当前进程的sessionID

ps ajx命令查看系统中的进程
参数a表示不仅列出当前用户的进程,也列出所有其他用户的进程
参数x表示不仅列出控制终端的进程，也列出所有无控制终端的进程
参数j表示列出与作业控制相关的信息。
*/

int main()
{
	printf("当前进程的PID为%d\r\n", getpid());
	printf("当前进程的会话ID为%d\r\n", getsid(0));	
	return 0;
} 
