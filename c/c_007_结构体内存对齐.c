#include <stdio.h>

/*
结构体内存分配原则

原则一：结构体中元素按照定义顺序存放到内存中，但并不是紧密排列。从结构体存储的首地址开始 ，每一个元素存入内存中时，它都会认为内存是以自己的宽度来划分空间的，因此元素存放的位置一定会在自己大小的整数倍上开始。

原则二： 在原则一的基础上，检查计算出的存储单元是否为所有元素中最宽的元素长度的整数倍。若是，则结束；否则，将其补齐为它的整数倍。
*/

typedef struct t1{
  int a; //4
  char b; //1
  double c; //8
}T1;

typedef struct t2{
  char a; //1
  double b; //8
  int c; //4
}T2;

typedef struct t3{
  double b; //8
  char a; //1
  int c; //4
  int d; //4
}T3;

typedef struct t4{
  double a; //8
  T1 t; //16
  int c; //4
}T4;

int main()
{
  printf("%d\r\n", sizeof(T1)); //16
  printf("%d\r\n", sizeof(T2)); //24
  printf("%d\r\n", sizeof(T3)); //24
  printf("%d\r\n", sizeof(T4)); //32

  return 0;
}
