#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
  int fd = open("./shared", O_RDWR|O_TRUNC|O_CREAT, 0664);
  if(fd==-1)
  {
    perror("open failed");
    exit(-1);
  }
  ftruncate(fd, 10);
  char* memp = mmap(NULL, 10, PROT_WRITE, MAP_SHARED, fd, 0);
  if(memp==MAP_FAILED)
  {
    perror("mmap failed");
    exit(-2);
  }
  close(fd);

  while(1)
  {
    static int id = 0;
    char str[20];
    sprintf(str, "hello-%d\t", id++);
    strcpy(memp, str);
    sleep(1);
  }

  munmap(memp, 10);
  return 0;
}
