#include <stdio.h>
#include <unistd.h>

/*
#include <unistd.h>
int chdir(const char* path);
chdir改变当前工作路径
返回值：成功返回0，失败返回-1
*/

int main()
{
  if(chdir("..")==0)
  {
    printf("改变当前工作目录成功\r\n");
  }
  return 0;
}
