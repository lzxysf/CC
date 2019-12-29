#include <stdio.h>
#include <stdlib.h>

/*
int setenv(const char *name, const char *value, int overwrite);
如果overwrite为0表示添加（如果没有name就添加，如果有就不动，都会返回成功）
如果overwrite为1标识覆盖（如果没有name就添加，如果有就覆盖掉，都会返回成功）
返回0为成功，-1位失败
*/

int main()
{
  setenv("PATH", "test", 0);
  // setenv("PATH", "test", 1);

  char* val = getenv("PATH");
  printf("myname=%s\r\n", val);
  return 0;
}
