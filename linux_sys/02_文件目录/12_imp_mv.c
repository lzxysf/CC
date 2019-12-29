#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  if(argc!=3)
  {
    return -1;
  }

  if(link(argv[1],argv[2])==-1)
  {
    perror("link file error:");
    return -1;
  }
  if(unlink(argv[1])==-1)
  {
    perror("unlink file error");
  }

  return 0;
}
