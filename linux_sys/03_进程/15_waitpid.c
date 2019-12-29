#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
进程的等待
#include <sys/types.h> 
#include <sys/wait.h>

wait(),waitpid()区别：
在一个子进程终止前，wait使其调用者阻塞，而waitpid有一个选项，可使调用者不阻塞;
waitpid()并不等待在其调用之后的第一个终止的子进程，它有若干个选项，可以控制它所等待的进程;
--------------------------------------------------------------------------------------
wait()
pid_t wait(int *status); 

返回值：
成功	成功结束运行的子进程的进程号
失败	返回-1

参数：
参数status如果不是一个空指针,则终止进程的终止状态就存放在statloc所指向的单元。
参数status如果是一个空指针,则表示父进程不关心子进程的终止状态

--------------------------------------------------------------------------------------
waitpid()
pid_t waitpid(pid_t pid, int *status, int options);

参数：
pid>0	只等待进程ID等于pid的子进程，不管其它已经有多少子进程运行结束退出了，只要指定的子进程还没有结束，waitpid就会一直等下去。
pid=-1	等待任何一个子进程退出，没有任何限制，此时waitpid和wait的作用一模一样。
pid=0时	等待同一个进程组中的任何子进程，如果子进程已经加入了别的进程组，waitpid不会对它做任何理睬。
pid<-1	等待一个指定进程组中的任何子进程，这个进程组的ID等于pid的绝对值。

参数status如果不是一个空指针,则终止进程的终止状态就存放在status所指向的单元
参数status如果是一个空指针,则表示父进程不关心子进程的终止状态

option
WNOHANG	若由pid指定的子进程未发生状态改变(没有结束)，则waitpid()不阻塞，立即返回0
WUNTRACED	返回终止子进程信息和因信号停止的子进程信息
WCONTINUED	返回收到SIGCONT信号而恢复执行的已停止子进程状态信息

返回值：
成功	      成功结束运行的子进程的进程号
失败	      返回-1
WNOHANG	    没有子进程退出返回0
*/

int main()
{
  pid_t pid1 = fork();
  if(pid1==0)
  {
    printf("子进程%d创建成功\r\n", getpid());
    int n = 3;
    while(n--)
    {
      printf("hello test\r\n");
      sleep(1);
    }
    exit(0);
  }

  pid_t pid2 = fork();
  if(pid2==0)
  {
    printf("子进程%d创建成功\r\n", getpid());
    char* str = "iamadog";
    str[0] = 'a';//段错误
    exit(0);
  }

#if 0
  int status2;
  pid_t p2 = waitpid(pid2, &status2, 0);
  printf("子进程%d(%d)结束了\r\n", p2, pid2);

  int status1;
  pid_t p1 = waitpid(pid1, &status1, 0);
  printf("子进程%d(%d)结束了\r\n", p1, pid1);
#else
  int status;
  pid_t pid;
  while(1)
  {
    //WNOHANG指的是非阻塞的，如果指定的进程没有结束，直接返回0
    if((pid = waitpid(-1, &status, WNOHANG)) >0)
    {
      printf("子进程%d结束了\r\n", pid);
    }
    usleep(10000);
  }
#endif

  return 0;
}
