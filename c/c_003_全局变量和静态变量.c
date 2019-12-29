#include <stdio.h>

//全局变量和静态变量，定义但没有初始化，会自动初始化为0
//其它变量不保证

int a;
int main()
{
  static int b;
  char* c;
  printf("a=%d,b=%d,c=%p", a, b, c);
}
