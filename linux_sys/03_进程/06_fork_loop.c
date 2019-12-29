#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
循环创建5个子进程,不能直接将fork放入循环中，
因为子进程也会继续创建子进程，因此要想创建5个子进程，需要把子进程中的循环终止
如下代码产生的进程数为2^n-1
for(int i = 0; i < n; n++)
{
  fork();
}
*/

int main()
{
  for(int i = 0; i < 5; i++)
  {
    pid_t id = fork();
    if(id == -1)
    {
      perror("fork error:");
      exit(1);
    }
    //如果是子进程就不再创建
    else if(id == 0)
    {
      printf("创建的进程,PID:%d,父进程:PPID:%d\r\n", getpid(), getppid());
      break;
    }
    else if(id > 0)
    {
      wait();
    }
  }
  
  return 0;
}
