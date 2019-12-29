#include <stdio.h>
#include <sys/stat.h>

/*
#include <sys/stat.h>
int stat(const char *pathname, struct stat *buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *pathname, struct stat *buf);
功能：获取文件属性
返回值：成功为0，失败为-1
stat是一个穿透函数，即对软链接文件进行操作时，操作的是链接到对那个文件而不是软链接文件本身
fstat和stat效果是一样的，只是fstat传入的是文件描述符fd，stat传入的是文件路径
lstat是不穿透对，即对软链接文件进行操作时，操作的是软链接文件本身
*/

int main()
{
  const char* path = "../test";
  struct stat statbuf;
  lstat(path, &statbuf);
  if(S_ISREG(statbuf.st_mode))
  {
    printf("常规文件\r\n");
  }
  else if(S_ISDIR(statbuf.st_mode))
  {
    printf("目录文件\r\n");
  }
  else if(S_ISLNK(statbuf.st_mode))
  {
    printf("链接文件\r\n");
  }
  else if(S_ISCHR(statbuf.st_mode))
  {
    printf("字符设备\r\n");
  }
  else if(S_ISBLK(statbuf.st_mode))
  {
    printf("块设备\r\n");
  }
  else if(S_ISFIFO(statbuf.st_mode))
  {
    printf("FIFO文件\r\n");
  }
  else if(S_ISSOCK(statbuf.st_mode))
  {
    printf("socket文件\r\n");
  }
}
