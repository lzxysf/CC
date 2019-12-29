#include <stdio.h>

int main()
{
  unsigned int a = -2, b = 3000;;
  if(a>0) //a是大于0的，因为定义的是无符号数。无符号数的打印要用%u
  {
    printf("hello\r\n");
    printf("%u\r\n", a);
  }
  if(a > b)//因为a实际为4294967294，所有a>b
  {
    printf("a>b");
  }
  return 0;
}
