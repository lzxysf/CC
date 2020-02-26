#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
int pthread_join(pthread_t thread, void** retval);
成功：0；失败：错误号
参数thread：线程ID
参数retval：存储线程结束状态

功能：阻塞等待线程退出，回收线程，获取线程退出状态。
当线程函数自己返回退出或pthread_exit退出时都不会释放线程所占用的堆栈和线程描述符（总计8K多），只有当调用了pthread_join后这些资源才会被释放。
当然，也可以提前使用pthread_detach将线程与控制线程分离，这样线程退出后就不需要进行回收，详细可见pthread_detach函数讲解。

调用该函数的线程将挂起等待，知道id为thread的线程终止。thread线程以不同的方法终止，得到的终止状态是不同的，总结如下：
1、如果thread线程通过return返回，retval所指向的单元里存放的是thread线程函数的返回值。
2、如果thread线程被别的线程调用pthread_cancel异常终止掉，retval所指向的单元里存放的是常数PTHREAD_CANCELD。
3、如果thread线程是自己调用pthread_exit终止的，retval所指向的单元存放的是pthread_exit携带的参数。
4、如果对thread线程的终止状态不感兴趣，可以传NULL给retval参数。
*/


typedef struct{
	int a;
	int b;
}exit_t;

void* thread_func(void *arg)
{
	printf("新创建的线程ID为%lu\r\n", pthread_self());
	sleep(2);
	printf("end\r\n");

	exit_t *ret;
	ret = (exit_t*)malloc(sizeof(exit_t));
	ret->a = 100;
	ret->b = 300;
	pthread_exit((void*)ret);
	
	return NULL;
}

int main()
{
	pthread_t tid;
	exit_t *retval;	

	pthread_create(&tid, NULL, thread_func, NULL);
	pthread_join(tid, (void**)&retval);
	
	printf("---------------------------------------\r\n");
	printf("子线程退出，退出参数1为%d，参数2为%d\r\n", retval->a, retval->b);
	return 0;
}
