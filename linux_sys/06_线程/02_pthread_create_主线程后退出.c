#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void*(*start_routine)(void*), void *arg)
创建一个新线程。
返回值：成功：0；失败：错误号--Linux下，所有线程操作的特点，失败均返回错误号。

参数1：传输参数，保存系统为我们分配好的线程ID
参数2：通常传NULL，表示使用线程默认属性。若想使用具体属性也可以修改该参
参数3：函数指针，指向线程主体函数（线程体），该函数运行结束，则线程结束
参数4：线程主函数执行期间所使用的参数
*/

void *thread_func(void* arg)
{
	printf("新线程创建成功\r\n");
	printf("新线程的线程ID为%lu\r\n", pthread_self());

	printf("end\r\n");

	return NULL;
}

int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, thread_func, NULL);
	
	sleep(1);//主线程等待新线程结束，主线程后退出。正常执行。

	return 0;
}

