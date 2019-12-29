#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
wait函数
一个进程在终止时会关闭所有文件描述符，释放在用户空间分配的内存，但它的PCB还保留着，内核在其中保存了一些信息：如果是正常终止则保存着退出状态，如果是异常终止则保存着导致该进程终止的信号是哪个。这个进程的父进程可以调用wait或waitpid获取这些信息，然后彻底清除掉这个进程。我们知道一个进程的退出状态可以在Shell中用特殊变量$?查看，因为Shell是它的父进程，当它终止时Shell调用wait或waitpid得到它的退出状态同时彻底清除掉这个进程。
父进程调用wait函数可以回收子进程终止信息。该函数有三个功能：
① 阻塞等待子进程退出 
② 回收子进程残留资源 
③ 获取子进程结束状态(退出原因)。
pid_t wait(int *status); 
成功：清理掉的子进程ID；失败：-1 (没有子进程)
当进程终止时，操作系统的隐式回收机制会：1.关闭所有文件描述符 2. 释放用户空间分配的内存。内核的PCB仍存在。其中保存该进程的退出状态。(正常终止→退出值；异常终止→终止信号)
可使用wait函数传出参数status来保存进程的退出状态。借助宏函数来进一步判断进程终止的具体原因。宏函数可分为如下三组：
 1.  WIFEXITED(status) 为非0	→ 进程正常结束
	WEXITSTATUS(status) 如上宏为真，使用此宏 → 获取进程退出状态 (exit的参数)
 2. 	WIFSIGNALED(status) 为非0 → 进程异常终止
	WTERMSIG(status) 如上宏为真，使用此宏 → 取得使进程终止的那个信号的编号。
*3. 	WIFSTOPPED(status) 为非0 → 进程处于暂停状态
	WSTOPSIG(status) 如上宏为真，使用此宏 → 取得使进程暂停的那个信号的编号。
	WIFCONTINUED(status) 为真 → 进程暂停后已经继续运行

要注意，一个wait只能捕获一个子进程的结束，要想捕获多个子进程的结束，请多次调用或使用循环
*/

int main()
{
  pid_t pid = fork();
  if(pid == 0)
  {
    printf("i am child process, pid=%d\r\n", getpid());
    sleep(2);
  }
  else if(pid > 0)
  {
    printf("i am parent process\r\n");
    int status;
    int ret = wait(&status);
    printf("子进程结束了，结束的子进程的pid为:%d\r\n", ret);
  }

  return 0;
}