#include <stdio.h>
#include <string.h>

/*
fscanf也能读取字符串，但是遇到第一个空格或换行时停止读取
*/

int main()
{
  FILE* fp = fopen("./test.txt", "r");
  char buf[255];
  fscanf(fp, "%s", buf);
  printf(buf);
  return 0;
}
