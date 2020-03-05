#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*

本例中先让前4个哲学家拿筷子，第五个哲学家不拿任何筷子，这样就不会形成死锁，其它哲学家可以依次拿两双筷子完成就餐。
等四个哲学家都就餐完毕（实际上只要第四个就餐完毕，但还需要等第一个哲学家释放第一个筷子），此时第五个哲学家再拿筷子。

*/

pthread_t tid[5];
pthread_mutex_t mutex[5];

void *thread_func(void *arg)
{
	int i = (int)arg;

	int leftchopstick = i;
	int rightchopstick = (i + 1) % 5;
	
	if(i != 4)
	{
		pthread_mutex_lock(&mutex[leftchopstick]);
		usleep(1);
		pthread_mutex_lock(&mutex[rightchopstick]);
		printf("哲学家%d正在就餐\r\n", i);
		pthread_mutex_unlock(&mutex[rightchopstick]);
		pthread_mutex_unlock(&mutex[leftchopstick]);
	}
	else
	{
		for(int i = 0; i < 4; i++)
		{
			pthread_join(tid[i], NULL);
		}
		
		pthread_mutex_lock(&mutex[leftchopstick]);
		pthread_mutex_lock(&mutex[rightchopstick]);
		printf("哲学家%d正在就餐\r\n", i);
		pthread_mutex_unlock(&mutex[rightchopstick]);
		pthread_mutex_unlock(&mutex[leftchopstick]);
	}
							
	return NULL;
}

int main()
{
	for(int i = 0; i < 5; i++)
	{
		pthread_mutex_init(&mutex[i], NULL);			
	}	
	
	for(int i = 0; i < 5; i++)
	{
		pthread_create(&tid[i], NULL, thread_func, (void*)i);
	}

	pthread_join(tid[4], NULL);

	for(int i = 0; i < 5; i++)
	{
		pthread_mutex_destroy(&mutex[i]);
	}

	printf("全部就餐完毕\r\n");

	return 0;
}

