#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 5

void *thread_func(void *arg)
{
	sleep(1);
	printf("新创建线程的ID为%lu\r\n", pthread_self());
	return arg;
}

int main()
{
	pthread_t tid[N];	
	for(int i = 0; i < N; i++)
	{
		pthread_create(&tid[i], NULL, thread_func, (void*)i);	
	}
	for(int i = 0; i < N; i++)
	{
		void* retval;
		pthread_join(tid[i], &retval);
		printf("回收成功，回收参数为%d\r\n", (int)retval);
	}
	return 0;
}

