#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
  if(argc!=2)
  {
    printf("params error\r\n");
    return -1;
  }
  if(mkdir(argv[1], 0644)==-1)
  {
    perror("mkdir failed:");
    return -1;
  }
}
