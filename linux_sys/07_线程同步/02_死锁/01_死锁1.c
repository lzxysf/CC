/*
死锁的情况:
1.线程试图对同一个互斥量A加锁两次。
2.线程1拥有A锁，请求获得B锁；线程2拥有B锁，请求获得A锁。
*/

/*
本例中实现情况1，线程试图对同一个互斥量A枷锁两次。
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int main()
{
	pthread_mutex_t mutex_A;
	pthread_mutex_init(&mutex_A, NULL);
	
	pthread_mutex_lock(&mutex_A);
	printf("do something1\r\n");
	pthread_mutex_lock(&mutex_A);//此处再次尝试获得锁A，但是已经不可能获得了，因此会一直在此阻塞
	printf("do something2\r\n");
	pthread_mutex_unlock(&mutex_A);
	pthread_mutex_unlock(&mutex_A);
}
