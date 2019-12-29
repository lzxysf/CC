#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

//malloc(0)可以成功执行，它开辟了实际的内存空间，在当前操作系统中它开辟了24位内存空间
//通过malloc开辟内存空间，通常情况下开辟的空间大小比指定的大小要大，可以通过malloc_usable_size查看实际大小
//在实际编程中不应当使用malloc(0)，也不应当使用malloc开辟的额外空间。

int main()
{
  int* p = malloc(25);
  int size = malloc_usable_size(p);
  printf("p=%p, size=%d", p, size); //p=0x156c260, size=24
  return 0;
}
