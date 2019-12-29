#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

/*
pipe只能用作有血缘关系的进程间通信，
fifo可以用作没有关系的两个进程间通信
fifo也可以多写端，多读端
*/

int main()
{
  if(access("./myfifo", F_OK)==-1)
  {
    mkfifo("./myfifo", 0777);
  }

  int fd;
  char str[64];
  int id = 1;
  if((fd=open("./myfifo", O_WRONLY))==-1)
  {
    perror("open error");
    exit(-2);
  }

  while(1)
  {
    int n;
    sprintf(str, "fifoteststring,pid=%d,id=%d\t", getpid(), id++);
    if((n = write(fd, str, strlen(str)))>0)
    {
      printf("n=%d\r\n", n);
    }
    sleep(1);
  }
  
  close(fd);
  return 0;
}
