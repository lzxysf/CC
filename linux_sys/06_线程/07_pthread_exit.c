#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
void pthread_exit(void* retval);
退出当前线程。
参数：retval表示线程退出状态，通常传NULL。
使用exit()会导致进程退出，使用pthread_exit()只会退出当前线程，不会退出进程。
*/

void* thread_func(void *arg)
{
	printf("当前线程ID为%lu\r\n", pthread_self());
	sleep(1);
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
