#include <stdio.h>

typedef struct t1{
  int a : 1;
  int b : 1;
}T1;

typedef struct t2{
  int a : 8;
  int b : 8;
  int c : 8;
  int d : 8;
}T2;

typedef struct t3{
  int a : 8;
  int b : 8;
  int c : 8;
  int d : 8;
  int e : 1;
}T3;

int main()
{
  printf("%d\r\n", sizeof(T1)); //4
  printf("%d\r\n", sizeof(T2)); //4
  printf("%d\r\n", sizeof(T3)); //8
  return 0;
}
