#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
循环创建多个子线程，每个线程打印自己是第几个被创建的线程
*/

void *thread_func(void *arg)
{
	int n = (int)arg;
	printf("创建新线程，是第%d个创建的线程\r\n", n);
	return NULL;
}

int main()
{
	pthread_t tid;
	for(int i = 0; i < 5; i++)
	{
		pthread_create(&tid, NULL, thread_func, (void*)i);
	}

	sleep(1);
	return 0;
}

