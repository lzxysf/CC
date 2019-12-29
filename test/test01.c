#include <stdio.h>
#include <limits.h>

#if 0
int main()
{
  printf("%d", INT_MIN);
  return 0;
}
#endif

int main()
{
  char* str = " hello";
  if(*(str+6) == '\0')
  {
    printf("end1\r\n");
  }
  if(*(str+6) == 0)
  {
    printf("end2\r\n");
  }

  printf("%d\r\n", '\0');// 0
  printf("%d\r\n", ' '); // 32
  printf("%d\r\n", '0');// 48
}
