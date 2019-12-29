#include <stdio.h>

int main()
{
  FILE* fp = fopen("./test.txt", "w+");
  fputc('8', fp);
  fputs("this is test for fputs\r\n", fp);
  fprintf(fp, "this is test for fprintf\r\n");

  fclose(fp);
  return 0;
}