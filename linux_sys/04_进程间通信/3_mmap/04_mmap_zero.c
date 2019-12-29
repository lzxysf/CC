#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef struct
{
  int id;
  char* name;
}student;

int main()
{
  pid_t pid;
  int fd;
  char* memp;

  fd = open("/dev/zero", O_RDWR);
  if(fd==-1)
  {
    perror("open file error");
    exit(-1);
  }
  memp = mmap(NULL, 20, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if(memp==MAP_FAILED)
  {
    perror("mmap error");
    exit(-2);
  }
  close(fd);

  pid = fork();
  if(pid==-1)
  {
    perror("fork error");
    exit(-2);
  }
  else if(pid>0)
  {
    student s1 = {1, "liming"};
    while(1)
    {
      s1.id++;
      memcpy(memp, &s1, sizeof(student));
      sleep(1);
    }
  }
  else if(pid==0)
  {
    student* s = (student*)memp;
    while(1)
    {
      printf("stu,id=%d,name=%s\r\n", s->id, s->name);
      sleep(1);
    }
  }

  munmap(memp, 20);

  return 0;
}