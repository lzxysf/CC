#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
理论上来讲，new/delete malloc/free 在没有构造和析构方法情况下混合使用不会有问题，
但是还是建议不要混合使用。因为遇到过无法混合使用的情况。
*/

using namespace std;

int main()
{
  int* a = new int[100];
  delete [] a;

  int* b = (int*)malloc(100);
  free(b);

  printf("end");

  return 0;
}
