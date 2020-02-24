#include <stdio.h>
#include <unistd.h>

/*
pid_t getpgid(pid_t pid);
获取指定进程的进程组ID
成功：0；失败：-1，设置errno
如果pid=0，那么该函数作用和getpgrp一样
*/

int main()
{
	printf("进程的pid为%d，gid为%d\r\n", getpid(), getpgid(0));
	return 0;
}
