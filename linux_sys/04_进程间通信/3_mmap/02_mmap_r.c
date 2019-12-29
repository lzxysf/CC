#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
  int fd = open("./shared", O_RDONLY, 0664);
  if(fd==-1)
  {
    perror("open failed");
    exit(-1);
  }
  char* memp = mmap(NULL, 10, PROT_READ, MAP_SHARED, fd, 0);
  if(memp==MAP_FAILED)
  {
    perror("mmap failed");
    exit(-2);
  }
  close(fd);
  
  while(1)
  {
    printf("%s\r\n", memp);
    sleep(1);
  }

  munmap(memp, 10);
  return 0;
}
