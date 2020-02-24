#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
pthread_t pthread_self(void);

获取线程ID。必定成功，不会失败，返回值即线程ID号。
pthread_t类型，本质，在Linux下为无符号整数（%lu）,其它系统中可能是结构体实现的。

编译时，应当添加pthread库
gcc 01_pthread_self.c -lpthread 
*/

int main()
{
	pthread_t tid = pthread_self();
	printf("当前线程的线程ID为%lu\r\n",  tid);
	return 0;
}
