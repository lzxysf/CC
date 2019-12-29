#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/*
进程间通信-管道
父进程将标注输出重定向到管道写
子进程将标准输入重定向到管道读
*/

int main()
{
  int fd[2];
  pid_t pid;

  if(pipe(fd)==-1)
  {
    perror("pipe error");
    exit(-1);
  }

  pid = fork();
  if(pid == -1)
  {
    perror("fork error");
    exit(-2);
  }
  else if(pid>0)
  {
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    execlp("ls", "ls", "-l", NULL);
    close(fd[1]);
    wait(NULL);
  }
  else if(pid==0)
  {
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    execlp("wc", "wc", "-l", NULL);
    close(fd[0]);
  }
  return 0;
}

