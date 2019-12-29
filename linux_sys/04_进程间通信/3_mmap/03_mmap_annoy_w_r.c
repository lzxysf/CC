#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

/*
匿名映射只能用于在有血缘关系的进程间通信
*/

typedef struct
{
  int id;
  char* name;
}student;

int main()
{
  char *memp;
  memp = mmap(NULL, sizeof(student), PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
  if(memp==MAP_FAILED)
  {
    perror("mmap failed");
    exit(-1);
  }
  
  pid_t pid;
  pid = fork();
  if(pid==-1)
  {
    perror("fork error");
    exit(-1);
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

  munmap(memp, sizeof(student));
  
  return 0;
}
