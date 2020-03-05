#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
想要的结果是HELLOWORLD一起打印，helloworld一起打印。
但是由于共享、竞争而没有任何同步机制，导致产生与时间有关的错误，造成数据混乱。
*/

void *thread_func(void *arg)
{
	srand(time(NULL));
	while(1)
	{
		printf("hello\r\n");
		sleep(rand()%3);/*模拟长时间操作共享资源，导致cpu易主，产生与时间有关的错误*/
		printf("world\r\n");
		sleep(rand()%3);
	}

	return NULL;
}

int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, thread_func, NULL);
	
	srand(time(NULL));
	while(1)
	{
		printf("HELLO\r\n");
		sleep(rand()%3);//模拟耗时操作
		printf("WORLD\r\n");
		sleep(rand()%3);
	}
	
	return 0;
}
