/*
相较于mutex而言，条件变量可以减少竞争。
如果直接使用mutex，除了生产者和消费者之间要竞争互斥量外，消费者和消费者之间也会竞争互斥量，但当汇聚链表中没有数据时候，消费者之间竞争互斥锁是无意义的。
有了条件变量机制以后，只有生产者完成生产，才会引起消费者之间的竞争，提高了程序效率。
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct msg{
	int num;
	struct msg *next;
};

struct msg *head;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *producer_func(void *arg)
{
	while(1)
	{
		struct msg* item = (struct msg*)malloc(sizeof(struct msg));
		item->num = rand()%1000+1;
          	pthread_mutex_lock(&mutex);
         	item->next = head;
         	head = item;
         	printf("生产了一个msg，数值为%d\r\n", item->num);
         	pthread_mutex_unlock(&mutex);
          
          	if(head != NULL)
          	{       
                	 pthread_cond_signal(&cond);
         	}
		
		usleep(100000);
	}
}

void *consumer_func(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(head == NULL)
		{
			pthread_cond_wait(&cond, &mutex);
		}
		printf("消费了一个msg，数值为%d\r\n", head->num);
		head = head->next;
		pthread_mutex_unlock(&mutex);

		usleep(100000);
	}

	return NULL;
}

int main()
{
	pthread_t producer_tid, consumer_tid;
	
	pthread_create(&producer_tid, NULL, producer_func, NULL);
	pthread_create(&consumer_tid, NULL, consumer_func, NULL);
	
	pthread_join(producer_tid, NULL);
	pthread_join(consumer_tid, NULL);
		
	return 0;
}
