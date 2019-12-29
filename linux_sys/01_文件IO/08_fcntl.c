#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

//fcntl--file control
//int fcntl(int fd, int cmd, ... /* arg */ );
//cmd参数有F_GETFD、F_SETFD
//使用fcntl将阻塞读取改为非阻塞读取

int main()
{
  int fd;
  char buf[1024];
  int n;

  fd = open("/dev/tty", O_RDONLY);
  if(fd < 0)
  {
    perror("open file error");
  }
  int f_flag = fcntl(fd, F_GETFL);
  f_flag |= O_NONBLOCK;
  fcntl(fd, F_SETFL, f_flag);

  while((n = read(fd, buf, sizeof(buf))) < 0)
  {
    sleep(1);
  }
  write(STDOUT_FILENO, buf, n);

  return 0;
}
