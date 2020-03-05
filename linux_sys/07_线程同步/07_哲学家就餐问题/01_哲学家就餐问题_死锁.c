#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
五个哲学家围成一个圆桌吃饭，每个人左右两边都有一把筷子，有5支筷子，如何完成吃饭。

五个哲学家抽象成5个线程，5个筷子抽象成5个互斥锁，程序演绎如下。

五个哲学家都是先拿起左边的筷子，再拿起右边的筷子，这样会构成阻塞。
*/

pthread_mutex_t mutex[5];

void *thread_func(void *arg)
{
	int i = (int)arg;

	int leftchopstick = i;
	int rightchopstick = (i + 1) % 5;//第5个哲学家右边的筷子是第0个，因为是圆桌

	pthread_mutex_lock(&mutex[leftchopstick]);
	usleep(1);//先让每一个哲学家都拿到一个筷子，这样可以保证形成阻塞，如果没有延时，某个哲学家运行速度过快的话就会一次获得两个筷子完成就餐后释放，这样就构不成阻塞了。这样虽然不构成阻塞了，但是是不可靠的，无法预知的。
	pthread_mutex_lock(&mutex[rightchopstick]);
	printf("哲学家%d正在就餐\r\n", i);
	pthread_mutex_unlock(&mutex[rightchopstick]);
	pthread_mutex_unlock(&mutex[leftchopstick]);
							
	return NULL;
}

int main()
{
	pthread_t tid[5];
	
	for(int i = 0; i < 5; i++)
	{
		pthread_mutex_init(&mutex[i], NULL);			
	}	
	
	for(int i = 0; i < 5; i++)
	{
		pthread_create(&tid[i], NULL, thread_func, (void*)i);
	}

	for(int i = 0; i < 5; i++)
	{
		pthread_join(tid[i], NULL);
	}

	for(int i = 0; i < 5; i++)
	{
		pthread_mutex_destroy(&mutex[i]);
	}

	return 0;
}

