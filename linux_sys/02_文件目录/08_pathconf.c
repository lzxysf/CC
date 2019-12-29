#include <stdio.h>
#include <unistd.h>

//pathconf：获得文件或目录有关的限制值

int main()
{
  long l = pathconf("../test/test.txt", _PC_NAME_MAX);
  printf("%ld\r\n", l);
  return 0;
}
