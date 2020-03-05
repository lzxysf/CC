/*
规定：
有一个消费者、一个生产者
如果space中有一个货物，生产者不能生产，只能阻塞。
如果space中没有货物，消费者不能消费，只能等待数据。

这种情况其实和互斥锁是一样的，但是如果space可以存放多个货物，那么使用信号量就会很方便了。见下一个c文件。
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/*
情景是这样的，
假设有一个仓库space，刚开始是空的，这个仓库可以存放1个货物。
sem_canput初始值为1，表示当前仓库中还能放1个货物
sem_canget初始值为0，表示当前仓库中还能取0个货物
*/

sem_t sem_canput;
sem_t sem_canget;
int space = 0;

void *producer_func(void *arg)
{
	while(1)
	{
		sem_wait(&sem_canput);
		space++;
		printf("生产了一个,space=%d\r\n",space);
		sem_post(&sem_canget);
		sleep(1);
	}

	return NULL;
}

void *consumer_func(void *arg)
{
	while(1)
	{
		sem_wait(&sem_canget);
		space--;
		printf("消费了一个,space=%d\r\n",space);
		sem_post(&sem_canput);
		sleep(1);
	}

	return NULL;
}

int main()
{
	pthread_t producer_tid, consumer_tid;

	sem_init(&sem_canput, 0, 1);
	sem_init(&sem_canget, 0, 0);
	
	pthread_create(&producer_tid, NULL, producer_func, NULL);
	pthread_create(&consumer_tid, NULL, consumer_func, NULL);
	
	pthread_join(producer_tid, NULL);
	pthread_join(consumer_tid, NULL);
	
	sem_destroy(&sem_canput);
	sem_destroy(&sem_canget);		
	return 0;
}

