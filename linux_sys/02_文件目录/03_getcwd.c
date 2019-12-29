#include <stdio.h>
#include <unistd.h>

/*
#include <unistd.h>
char* getcwd(char* buf, size_t size);
获取当前工作目录
如果获取当前目录成功，buf被赋值为当前目录，返回值指向buf
如果获取当前目录失败，返回值为NULL
*/

int main()
{
  //linux文件名限制为255个英文字符
  char buf[255];
  char* temp;
  temp = getcwd(buf, sizeof(buf));
  printf("%p\r\n", buf);
  printf("%p\r\n", temp);
  printf("%s\r\n", buf);
  printf("%s\r\n", temp);

  if(chdir("..")==-1)//将当前工作目录更改为上级目录
  {
    perror("change dir error:");
    return -1;
  }
  temp = getcwd(buf, sizeof(buf));
  if(temp == NULL)
  {
    perror("get current directory error:");
    return -1;
  }
  printf("%s\r\n", temp);
  
  return 0;
}
