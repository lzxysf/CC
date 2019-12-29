#include <stdio.h>
#include <string.h>

int endwith(char* str, char* end);

int main()
{
  char* s = "hello world";
  int ret = endwith(s, "world");
  printf("是否是后缀:%d\r\n", ret);
}
