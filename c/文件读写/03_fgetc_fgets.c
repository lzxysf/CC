#include <stdio.h>
#include <string.h>

//char *fgets(char *s, int size, FILE *stream);
//fgets会读取size长度的字符并在其后加上\0返回，如果遇到文件末尾或换行则会结束，返回读取到的字符串包括换行符，

//fgetc发生错误或在文件末尾返回EOF(end of file,其值为-1)
//fgets发生错误或在文件末尾返回NULL

int main()
{
  FILE* fp = fopen("./test.txt", "r");
  int c = fgetc(fp);
  printf("c=%d\r\n", c);

  char buf[255];
  while(fgets(buf, 255, fp))
  {
    printf(buf);
  }

  return 0;
}
