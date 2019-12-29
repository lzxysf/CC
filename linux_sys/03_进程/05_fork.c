#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

/*
fork失败返回-1
成功，父进程返回子进程ID（大于0），子进程返回0

pid_t getpid(void); //获取当前进程ID
pid_t getppid(void); //获取父进程ID
*/

int main()
{
  pid_t id = fork();
  if(id == -1)
  {
    perror("fork error:");
  }
  else if(id > 0)
  {
    printf("i am parent process, pid is %d, parent pid is %d\r\n", getpid(), getppid());
    wait();
  }
  else if(id == 0)
  {
    printf("i am child process, pid is %d, parent pid is %d\r\n", getpid(), getppid());
  }

  getchar();
  
  return 0;
}
