#include <stdio.h>
#include <unistd.h> //open read write sleep
#include <fcntl.h>

#define TRY_MSG "trying...\r\n"

int main()
{
  int fd;
  char buf[64];
  int n;
  
  fd = open("/dev/tty", O_RDONLY | O_NONBLOCK);
  if(fd < 0)
  {
    perror("open tty error");
  }

  n = read(fd, buf, sizeof(buf));
  while(n < 0)
  {
    write(STDOUT_FILENO, TRY_MSG, sizeof(TRY_MSG));
    sleep(1);
    n = read(fd, buf, sizeof(buf));
  }

  write(STDOUT_FILENO, buf, n);

  return 0;
}
