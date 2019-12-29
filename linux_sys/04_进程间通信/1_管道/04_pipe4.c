#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*
创建两个子进程
父进程读
两个子进程同时写
*/

int main()
{
  int fd[2];
  pipe(fd);

  pid_t pid;
  int i = 0;
  for(i = 0; i < 2; i++)
  {
    pid = fork();
    if(pid == 0)
    {
      break;
    }
  }
  
  if(i==0)//子进程1
  {
    close(fd[0]);
    char str[50];
    sprintf(str, "hello pipe test, my pid = %d", getpid());
    while(1)
    {
      write(fd[1], str, strlen(str)+1);
      usleep(500000);
    }
    close(fd[1]);
  }
  else if(i==1)//子进程2
  {
    close(fd[0]);
    char str[50];
    sprintf(str, "hello pipe test, my pid = %d", getpid());
    while(1)
    {
      write(fd[1], str, strlen(str)+1);
      usleep(500000);
    }
    close(fd[1]);
  }
  else if(i==2)//父进程
  {
    close(fd[1]);
    char buf[64];
    while(1)
    {
      int size = read(fd[0], buf, sizeof(buf));
      printf("%s\r\n", buf);
    }
    close(fd[0]);

    wait(NULL);
  }

  return 0;
}
