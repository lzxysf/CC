#include <stdio.h>
#include <string.h>

int main()
{
  FILE* fp = NULL;
  char str[] = "this is test for fread and fwrite\r\n";
  char buf[64];
  
  fp = fopen("./test.txt", "w+");
  fwrite(str, strlen(str)+1, 1, fp);

  //此时文件指针位于文件末尾，需要定位到文件开头
  fseek(fp, 0, SEEK_SET);

  fread(buf, strlen(str)+1, 1, fp);

  printf("%s", buf); 
}
