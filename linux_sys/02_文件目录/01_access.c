#include <stdio.h>
#include <unistd.h> //access

/*
#include <unistd.h>

int access(const char* pathname, int mode)

F_OK:0,判断文件是否存在
X_OK:0,判断文件是否可执行
W_OK:1,判断文件是否可写
R_OK:2,判断文件是否可读

返回值:为真返回0,为假返回-1
*/

int main()
{
  char* path = "../test/test.txt";
  //判断文件是否存在
  int ret = access(path, F_OK);
  if(ret==0)
  {
    printf("file exists\r\n");
  }
  
  //判断文件是否可执行
  ret = access(path, X_OK);
  if(ret==0)
  {
    printf("文件可执行\r\n");
  }

  //判断文件是否可写
  ret = access(path, W_OK);
  if(ret==0)
  {
    printf("文件可写\r\n");
  }

  //判断文件是否可读
  ret = access(path, R_OK);
  if(ret == 0)
  {
    printf("文件可读\r\n");
  }

  return 0;
}
