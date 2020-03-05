#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
对两个竞争的线程进行线程同步。
*/

pthread_mutex_t mutex;

void *thread_func(void *arg)
{
	int num = 5;
	srand(time(NULL));
	while(num--)
	{
		pthread_mutex_lock(&mutex);
		printf("hello\r\n");
		sleep(rand()%3);/*模拟长时间操作共享资源，导致cpu易主，产生与时间有关的错误*/
		printf("world\r\n");
		sleep(rand()%3);
		pthread_mutex_unlock(&mutex);
		usleep(1000);
	}

	return NULL;
}

int main()
{
	int num = 5;
	pthread_t tid;

	pthread_mutex_init(&mutex, NULL);	
	
	pthread_create(&tid, NULL, thread_func, NULL);
	
	srand(time(NULL));

	while(num--)
	{
		pthread_mutex_lock(&mutex);
		printf("HELLO\r\n");
		sleep(rand()%3);//模拟耗时操作
		printf("WORLD\r\n");
		sleep(rand()%3);
		pthread_mutex_unlock(&mutex);
		usleep(1000);//1ms。如果不加延时，释放锁后由于在while循环中又马上获取锁，这样会导致子线程很长时间不能获得CPU时间。
	}

	pthread_join(tid, NULL);
	
	pthread_mutex_destroy(&mutex);
		
	return 0;
}
