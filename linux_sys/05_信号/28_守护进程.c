#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
Daemon（精灵）进程，是Linux中的后台服务进程，通常独立于控制终端并且周期性地执行某种任务或等待处理某些发生的事件。一般采用以结尾的名字。

Linux后台的一些系统服务进程，没有控制终端，不能直接与用户交互。不受用户登录、注销的影响，一直在运行着，他们都是守护进程。如：预读入缓输出机制的实现；ftp服务器；nfs服务器。

创建守护进程，最关键的一步是调用setsid()函数创建一个新的Session，并成为首进程。
*/

/*
创建守护进程模型，步骤：
1）创建子进程，父进程退出
所有工作在子进程中进行，形式上脱离了控制终端
2）在子进程中创建新会话
执行setsid()函数，使子进程完全独立出来，脱离控制
3）改变当前目录为根目录
chdir()。防止占用可卸载目录。也可以换做其它路径。
4）重设文件权限掩码
umask函数
因为创建函数时文件权限掩码会屏蔽掉一些对应的位，所以需要重建文件创建掩码
5）关闭文件描述符
继承的打开文件不会被用到，浪费系统资源，所以需要关闭
6）开始执行守护进程核心工作
7）守护进程退出
*/

/*
ps ajx | more 指令可以查看linux进程下的守护进程。可以看到守护进程的TPGID为-1， 也就是TPGID为-1的为守护进程。

gcc 28_守护进程.c -o mydaemon
之后执行ps ajx | more 就可以查看到mydaemon守护进程
*/

int main()
{
	pid_t pid;
	pid = fork();
	
	if(pid < 0)
	{
		perror("fork error");
	}
	if(pid > 0)
	{
		exit(0);//1.父进程退出
	}

	setsid();//2.在子进程中创建新会话

	chdir("/");//3.改变当前目录为根目录

	umask(0);//4.重设文件权限掩码
	
	//5.重定向文件描述符
	int fd = open("/dev/null", O_RDWR);
	dup2(fd, 0);
	dup2(fd, 1);	
	
	//6.执行守护进程核心工作
	while(1)
	{
		sleep(1);
	}
		
	return 0;
}
