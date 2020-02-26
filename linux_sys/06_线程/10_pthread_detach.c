#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

/*
int pthread_detach(pthread_t thread);
功能：实现线程分离。
返回值：成功返回0，失败返回错误号。

线程分离状态：指定该状态，线程主动与主控线程断开关系。线程结束后，其退出状态不由其它线程获取，而直接自己主动释放。网络、多线程服务器常用。
(如果进程有该机制，将不会导致僵尸进程。僵尸进程的产生主要是由于，进程死后，大部分资源被释放，但是一点残留资源仍存在于系统中，导致内核认为该进程仍然存在)

如果使用了pthread_detach将线程分离了，还使用pthread_join捕获线程就会不成功，pthread_join会直接返回EINVAL(22 Invalid argument)

1.与windows不同，pthread有两种状态joinable状态和unjoinable状态，如果线程是joinable状态，当线程函数自己返回退出时或pthread_exit时都不会释放线程所占用堆栈和线程描述符（总计8K多）。只有当你调用了pthread_join之后这些资源才会被释放。若是unjoinable状态的线程，这些资源在线程函数退出时或pthread_exit时自动会被释放。
2.unjoinable属性可以在pthread_create时指定，或在线程创建后在线程中pthread_detach自己, 如：pthread_detach(pthread_self())，将状态改为unjoinable状态，确保资源的释放。或者将线程置为 joinable,然后适时调用pthread_join.
3.其实简单的说就是在线程函数头加上 pthread_detach(pthread_self())的话，线程状态改变，在函数尾部直接 pthread_exit线程就会自动退出。省去了给线程擦屁股的麻烦。
*/

void *thread_func(void *arg)
{
	//pthread_detach(pthread_self());//也可以在子线程内部将当前线程分离	

	printf("子线程创建成功，线程ID是%lu\r\n", pthread_self());
	
	printf("end");
		
	return (void*)1;
}

int main()
{
	pthread_t tid;
	void* retval;
	int ret;	
	
	pthread_create(&tid, NULL, thread_func, NULL);		
	pthread_detach(tid);//将新创建的线程分离

	ret = pthread_join(tid, &retval);
	//表示没有执行成功，此时ret为errno=EINVAL(22)(Invalid argument)
	if(ret != 0)
	{
		fprintf(stderr, "thread %s\r\n", strerror(ret));
	}
	else
	{
		fprintf(stderr, "thread exit code %d\r\n", (int)retval);	
	}
		
	printf("主线程运行结束\r\n");
	
	sleep(1);//等待子线程运行完成。主线程的结束代表的是进程的结束，除非主线程使用pthread_exit()。

	return 0;
}

