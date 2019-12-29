#include <stdio.h>
#include <unistd.h>

/*
僵尸进程
子进程先于父进程结束，子进程的残留资源（PCB）存放于内核，变成僵尸进程
僵尸进程使不能用kill命令清除掉的，因为kill命令只是用来终止进程的，而僵尸进程已经终止
*/

int main()
{
  pid_t pid = fork();
  if(pid > 0)
  {
    while(1)
    {
      printf("i am parent process, son pid is %d\r\n", pid);
      sleep(1);
    }
  }
  else if(pid == 0)
  {
    printf("i am child process, ppid=%d\r\n", getppid());
    sleep(9);
  }
  else if(pid==-1)
  {
    perror("fork error");
    return -1;
  }
  return 0;
}
