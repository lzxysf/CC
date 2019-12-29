#include <stdio.h>
#include <string.h>

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
