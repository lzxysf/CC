#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    printf("参数输入不正确\r\n");
    return -1;
  }

  system("pwd");
  int ret = chdir(argv[1]);
  if(ret < 0)
  {
    perror("chdir failed:");
  }
  system("pwd");

  return 0;
}
