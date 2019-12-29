#include <stdio.h>
#include <stdlib.h>

/*
int unsetenv(const char *name);
如果name存在，删除这个环境变量，返回成功；如果name不存在，unsetenv什么也不做，返回成功。
成功返回0，失败返回-1

setenv和unsetenv都是对当前进程的环境变量进行改变，对系统其它进程没有影响。
*/

int main()
{
  unsetenv("PATH");
  char* path = getenv("PATH");
  printf("PATH=%s\r\n",path);
  return 0;
}
