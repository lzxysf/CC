#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
int pthread_cancel(pthread_t thread);
成功：0； 失败：错误号
功能：杀死（取消）线程

注意：线程的取消并不是实时的，而是有一定的延时。需要等待线程到达某个取消点。
取消点：是线程检查是否被取消，并按请求进行动作的一个位置。通常是一些系统调用。可粗略认为一个系统调用（进入内核）即为一个取消点。

被取消的线程，退出值为PTHREAD_CANCEL(-1)，因此使用pthread_join回收线程时获得的线程退出值为-1

*/
void* thread_func(void* arg)
{
	while(1)
	{
		printf("新线程的线程ID为%lu\r\n", pthread_self());
		sleep(1);
	}
}

int main()
{
	pthread_t tid;
	void *retval;
	
	pthread_create(&tid, NULL, thread_func, NULL);
	
	sleep(7);

	pthread_cancel(tid);
	
	pthread_join(tid, &retval);

	printf("child thread exit code is %d\r\n",(int)retval);
		
	return 0;
}

