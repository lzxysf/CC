#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_rwlock_t mylock;
int counter = 0;

void *write_thread(void *arg)
{
	int i = (int)arg;
	while(1)
	{
		pthread_rwlock_wrlock(&mylock);
		counter++;
		printf("写线程%d中的当前counter值为%d\r\n", i, counter);
		pthread_rwlock_unlock(&mylock);

		usleep(100000);
	}
	
	return NULL;
}

void *read_thread(void *arg)
{
	int i = (int)arg;
	while(1)
	{	
		pthread_rwlock_rdlock(&mylock);
		printf("读线程%d中的当前counter值为%d\r\n", i, counter);
		pthread_rwlock_unlock(&mylock);

		usleep(10000);
	}	
	return NULL;
}

int main()
{
	pthread_t tid[8];
	
	pthread_rwlock_init(&mylock, NULL);
		
	//创建3个写线程
	for(int i = 0; i < 3; i++)
	{
		pthread_create(&tid[i], NULL, write_thread, (void*)i);
	}
	//创建5个读线程
	for(int i = 3; i < 8; i++)
	{
		pthread_create(&tid[i], NULL, read_thread, (void*)i);
	}
	
	//线程回收
	for(int i = 0; i < 8; i++)
	{
		pthread_join(tid[i], NULL);	
	}
	
	pthread_rwlock_destroy(&mylock);
	return 0;
}
