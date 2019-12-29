#include <stdio.h>
#include "add.h"
#include "multi.h"

int main()
{
  int ret = add(2, 5);
  printf("sum=%d\r\n", ret);

  ret = multi(3, 8);
  printf("multi=%d\r\n", ret);
}
