#include <stdio.h>
#include <unistd.h>

/*
孤儿进程
父进程先于子进程结束，则子进程成为孤儿进程，子进程的父进程成为init进程，称为init进程领养孤儿进程。
init进程的pid为1
*/

int main()
{
  pid_t pid = fork();
  if(pid > 0)
  {
    printf("i am parent process\r\n");
    sleep(9);
  }
  else if(pid == 0)
  {
    while(1)
    {
      printf("i am child process, pid=%d, ppid=%d\r\n", getpid(), getppid());
      sleep(1);
    }
  }
  else if(pid==-1)
  {
    perror("fork error");
    return -1;
  }
  return 0;
}
