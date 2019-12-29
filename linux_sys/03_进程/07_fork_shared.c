#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

/*
父子进程之间在fork后。有哪些相同，那些相异之处呢？

刚fork之后：
父子相同处: 全局变量、.data、.text、栈、堆、环境变量、用户ID、宿主目录、进程工作目录、信号处理方式...
父子不同处: 1.进程ID   2.fork返回值   3.父进程ID    4.进程运行时间    5.闹钟(定时器)   6.未决信号集

似乎，子进程复制了父进程0-3G用户空间内容，以及父进程的PCB，但pid不同。真的每fork一个子进程都要将父进程的0-3G地址空间完全拷贝一份，然后在映射至物理内存吗？

当然不是!父子进程间遵循“读时共享，写时复制”的原则。这样设计，无论子进程执行父进程的逻辑还是执行自己的逻辑都能节省内存开销。
*/

//这个例子中，虽然主进程获关闭了文件描述符fd，但是子进程没有关闭，因此在子进程中仍然可以使用fd进行文件操作

int main()
{
  int fd;
  char buf[64];
  int size;

  fd = open("../test/text.txt", O_RDONLY);
  if(fd==-1)
  {
    perror("open file error:");
    exit(1);
  }

  pid_t pid = fork();
  
  if(pid > 0)
  {
    close(fd);
  }
  else if(pid == 0)
  {
    sleep(1);
    while((size = read(fd, buf, sizeof(buf))) > 0)
    {
      write(STDOUT_FILENO, buf, size);
    }
  }
  else if(pid == -1)
  {
    perror("fork error:");
    exit(2);
  }

  return 0;
}
