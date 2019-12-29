#include <stdio.h>
#include <string.h>

int endwith(char* str, char* end);

int main(int argc, char* argv[])
{
  char* s = "hello world";
  // int ret = endwith(s, "world");
  int ret = endwith(argv[1], argv[2]);
  printf("是否是后缀:%d\r\n", ret);
}

int endwith(char* str, char* end)
{
  int len1 = strlen(str);
  int len2 = strlen(end);

  if(len1 < len2)
  {
    return 0;
  }

  while(len2 >= 0)
  {
    if(*(str+len1) != *(end+len2))
    {
      return 0;
    }
    len1--;
    len2--;
  }
  return 1;
}
