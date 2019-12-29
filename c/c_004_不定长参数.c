#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void test(int num_args, ...)
{
  va_list va;
  va_start(va, num_args);
  for(int i = 0; i < num_args; i++)
  {
    char* val = va_arg(va, char*);
    printf("val_%d=%s\r\n", i, val);
  }
  va_end(va);
}

int main()
{
  test(3, "hello","hbfec","sdk");
  return 0;
}
