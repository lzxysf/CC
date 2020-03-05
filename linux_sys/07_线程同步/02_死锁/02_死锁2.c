/*
死锁的情况:
1.线程试图对同一个互斥量A加锁两次。
2.线程1拥有A锁，请求获得B锁；线程2拥有B锁，请求获得A锁。
*/

/*
本例中实现情况2,线程1拥有A锁，请求获得B锁，线程2拥有B锁，请求获得A锁
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex_A, mutex_B;

void *thread_func(void* arg)
{
	pthread_mutex_lock(&mutex_B);//线程2获得B锁
	sleep(1);
	printf("do something3\r\n");
	pthread_mutex_lock(&mutex_A);//线程2请求获得A锁，无法获取会被一直阻塞
	printf("do something4\r\n");
	pthread_mutex_unlock(&mutex_A);
	pthread_mutex_unlock(&mutex_B);

	return NULL;
}

int main()
{
	pthread_t tid;
	
	pthread_mutex_init(&mutex_A, NULL);
	pthread_mutex_init(&mutex_B, NULL);
	
	pthread_create(&tid, NULL, thread_func, NULL);	

	pthread_mutex_lock(&mutex_A);//线程1获得A锁
	sleep(1);
	printf("do something1\r\n");
	pthread_mutex_lock(&mutex_B);//线程1请求获得B锁，无法获取会被一直阻塞
	printf("do something2\r\n");
	pthread_mutex_unlock(&mutex_B);
	pthread_mutex_unlock(&mutex_A);
	
	pthread_join(tid, NULL);
	pthread_mutex_destroy(&mutex_A);
	pthread_mutex_destroy(&mutex_B);
	
	return 0;
}
