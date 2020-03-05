/*space中可以存放多个货物，此时sem信号量机制就能体现其价值*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem_canput;
sem_t sem_canget;

int space = 0;

void *producer_thread(void *arg)
{
	while(1)
	{
		sem_wait(&sem_canput);
		space++;
		printf("生产了一个，当前space为%d\r\n", space);
		sem_post(&sem_canget);
		sleep(rand()%3+1);
	}
		
	return NULL;
}

void *consumer_thread(void *arg)
{
	while(1)
	{
		sem_wait(&sem_canget);
		space--;
		printf("消费了一个，当前space为%d\r\n", space);
		sem_post(&sem_canput);
		sleep(rand()%3+1);
	}
	
	return NULL;
}

int main()
{
	pthread_t producer_tid, consumer_tid;

	srand(time(NULL));
	
	sem_init(&sem_canput, 0, 2);//初始可以存放的货物是2，即仓库space可存放2个货物。
	sem_init(&sem_canget, 0, 0);

	pthread_create(&producer_tid, NULL, producer_thread, NULL);
	pthread_create(&consumer_tid, NULL, consumer_thread, NULL);
	
	pthread_join(producer_tid, NULL);
	pthread_join(consumer_tid, NULL);
		
	sem_destroy(&sem_canput);
	sem_destroy(&sem_canget);
	
	return 0;
}

