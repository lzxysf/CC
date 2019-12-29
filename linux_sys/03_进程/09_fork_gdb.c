#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

/*
使用gdb调试多进程程序的方法

1.follow-fork-mode
set follow-fork-mode parent         设置调试父进程
set follow-fork-mode child          设计调试子进程
进入gdb后，直接设置，默认是parent
如果想要调试子进程，进入gdb后设置set follow-fork-mode child
可用使用show follow-fork-mode 来查询当前fork模式

2.detach-on-fork
set detach-on-fork on
只调试父进程或子进程的其中一个（根据follow-fork-mode决定），这是默认的模式

set detach-on-fork off
父子进程都处于gdb的控制之下，其中一个进程正常调试（根据follow-fork-mode决定），另一个进程暂停执行
*/

int main()
{
  pid_t id = fork();
  
  if(id > 0)
  {
    printf("i am parent process, pid is %d, parent pid is %d\r\n", getpid(), getppid());
    printf("test1-1\r\n");
    sleep(1);
    printf("test1-2\r\n");
    sleep(1);    
    printf("test1-3\r\n");
    wait();
  }
  else if(id == 0)
  {
    printf("i am child process, pid is %d, parent pid is %d\r\n", getpid(), getppid());
    printf("test2-1\r\n");
    sleep(1);    
    printf("test2-2\r\n");
    sleep(1);    
    printf("test2-3\r\n");
  }

  return 0;
}
