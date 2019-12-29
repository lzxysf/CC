#include <stdio.h>
#include <stdlib.h> //getenv

/*
成功返回对应环境变量的值
失败返回NULL
*/

int main()
{
  char* val = getenv("PATH");
  printf("PATH=%s\r\n", val);
  return 0;
}
