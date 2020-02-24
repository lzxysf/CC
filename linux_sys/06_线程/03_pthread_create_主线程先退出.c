#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*

主线程先退出，新线程也会立即退出，系统清除所有资源。

*/

void *thread_func(void* arg)
{
	printf("新线程创建成功\r\n");
	printf("新线程的线程ID为%lu\r\n", pthread_self());
	
	sleep(2);
	
	printf("end\r\n");

	return NULL;
}

int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, thread_func, NULL);
	sleep(1);
	return 0;
}

