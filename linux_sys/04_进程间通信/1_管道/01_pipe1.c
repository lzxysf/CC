#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/*
进程间通信-管道
父进程写数据，子进程读数据
*/

int main()
{
  pid_t pid;
  char buf[64];
  int fd[2];
  if(pipe(fd) == -1)
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
  else if(pid > 0) //父进程，用来写
  {
    close(fd[0]);
    char* str = "pipe test string";
    sleep(1);
    write(fd[1], str, strlen(str)+1);
    close(fd[1]);
    wait(NULL);
  }
  else if(pid == 0)//子进程，用来读
  {
    close(fd[1]);
    //此处，read是阻塞的，父进程在1s后写管道，子进程会在此处阻塞直到从管道中有数据可以取出
    int size = read(fd[0], buf, sizeof(buf));
    write(STDOUT_FILENO, buf, size);
    close(fd[0]);
  }
  return 0;
}
