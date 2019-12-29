#include <stdio.h>
#include <stdlib.h>

/*
free一个空指针不会崩溃,参数为空不执行任何操作
*/

int main()
{
  char* abc = NULL;
  free(abc);

  printf("test");
  return 0;
}