#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*

主线程创建线程B，线程B创建线程C。线程B退出后进程不会退出，所以线程C仍会继续执行。

*/

void *thread_func2(void* arg)
{
	printf("线程C创建成功\r\n");
	printf("线程C的线程ID为%lu\r\n", pthread_self());
	
	sleep(2);
	
	printf("end\r\n");
	return NULL;
		
}

void *thread_func1(void* arg)
{
	printf("线程B创建成功\r\n");
	printf("线程B的线程ID为%lu\r\n", pthread_self());

	pthread_t tid;
	pthread_create(&tid, NULL, thread_func2, NULL);
	
	return NULL;
}

int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, thread_func1, NULL);
	
	pthread_exit(NULL);

	return 0;
}

