#include <stdio.h>
#include <unistd.h>

void start(int val1, int val2)
{
  printf("wrapper start\r\n");
  printf("val1=%d, val2=%d\r\n", val1, val2);
}

typedef void (*wrapper_start_func)(int val1, int val2);

int main()
{
  wrapper_start_func wrapper_start = start;
  wrapper_start(100, 200);
  return 0;
}
