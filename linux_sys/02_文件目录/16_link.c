#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/*
#include <unistd.h>
int link(const char* oldpath, const char* newpath);
link对一个已经存在的文件创建一个硬链接，如果newpath已经存在就不会创建

int unlink(const char* pathname);
unlink()函数是删除一个dentry   
执行unlink()函数并不一定会真正的删除文件，它先会检查文件系统中此文件的连接数是否为1，如果不是1说明此文件还有其他链接对象，因此只对此文件的连接数进行减1操作。
若连接数为1，并且在此时没有任何进程打开该文件，此内容才会真正地被删除掉。在有进程打开此文件的情况下，则暂时不会删除，直到所有打开该文件的进程都结束时文件就会被删除。
*/

int main()
{
  //创建硬链接
  if(link("../test/test.txt", "../test/hardtest.txt") < 0)
  {
    perror("link file error:");
    return -1;
  }
  printf("link success\r\n");
  //删除文件
  unlink("../test/test.txt");
  
  return 0;
}
