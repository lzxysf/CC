#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main()
{
  if(access("./myfifo", F_OK)==-1)
  {
    mkfifo("./myfifo", 0777);
  }

  int fd;
  char buf[64];
  int size;
  if((fd=open("./myfifo", O_RDONLY))==-1)
  {
    perror("open error");
    exit(-3);
  }
  while(1)
  {
    size=read(fd, buf, sizeof(buf));
    write(STDOUT_FILENO, buf, size);
    sleep(1);
  }

  return 0;
}
