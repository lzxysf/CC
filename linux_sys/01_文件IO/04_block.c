#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
  int fd;
  int n;
  char buf[64];

  fd = open("/dev/tty", O_RDONLY);

  n = read(fd, buf, 64);

  if(n > 0)
  {
    write(STDOUT_FILENO, buf, n);
  }

  close(fd);

  return 0;
}
