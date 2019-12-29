#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*
创建两个子进程
父进程读
两个子进程同时写
一段时间后写端文件描述符都关闭，此时读端read会返回0，就像读到文件末尾一样

使用管道需要注意以下4种特殊情况（假设都是阻塞I/O操作，没有设置O_NONBLOCK标志）：
1. 如果所有指向管道写端的文件描述符都关闭了（管道写端引用计数为0），而仍然有进程从管道的读端读数据，那么管道中剩余的数据都被读取后，再次read会返回0，就像读到文件末尾一样。
2. 如果有指向管道写端的文件描述符没关闭（管道写端引用计数大于0），而持有管道写端的进程也没有向管道中写数据，这时有进程从管道读端读数据，那么管道中剩余的数据都被读取后，再次read会阻塞，直到管道中有数据可读了才读取数据并返回。
3. 如果所有指向管道读端的文件描述符都关闭了（管道读端引用计数为0），这时有进程向管道的写端write，那么该进程会收到信号SIGPIPE，通常会导致进程异常终止。当然也可以对SIGPIPE信号实施捕捉，不终止进程。具体方法信号章节详细介绍。
4. 如果有指向管道读端的文件描述符没关闭（管道读端引用计数大于0），而持有管道读端的进程也没有从管道中读数据，这时有进程向管道写端写数据，那么在管道被写满时再次write会阻塞，直到管道中有空位置了才写入数据并返回。
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
    int count = 5;
    while(count--)
    {
      write(fd[1], str, strlen(str)+1);
      sleep(1);
    }
    close(fd[1]);
  }
  else if(i==1)//子进程2
  {
    close(fd[0]);
    char str[50];
    sprintf(str, "hello pipe test, my pid = %d", getpid());
    int count = 5;
    while(count--)
    {
      write(fd[1], str, strlen(str)+1);
      sleep(1);
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
      if(size==0)
      {
        break;
      }
      printf("%s\r\n", buf);
    }
    close(fd[0]);

    wait(NULL);
    wait(NULL);

    printf("运行结束");
  }

  return 0;
}
