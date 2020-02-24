#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*

主线程使用pthread_exit退出，相当于只是线程退出不是进程退出。那么新线程仍会继续执行。

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
	
	pthread_exit(NULL);

	return 0;
}

