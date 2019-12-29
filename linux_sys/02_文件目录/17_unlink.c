/*
int unlink(const char* pathname);
unlink()函数是删除一个dentry   
执行unlink()函数并不一定会真正的删除文件，它先会检查文件系统中此文件的连接数是否为1，如果不是1说明此文件还有其他链接对象，因此只对此文件的连接数进行减1操作。
若连接数为1，并且在此时没有任何进程打开该文件，此内容才会真正地被删除掉。在有进程打开此文件的情况下，则暂时不会删除，直到所有打开该文件的进程都关闭了该文件文件才会被删除。
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct {
  int a;
  int b;
}test;

int main()
{
  int fd;
  char buf[1024];

  fd = open("../test/test.txt", O_RDWR);
  if(fd == -1)
  {
    perror("open failed");
  }

  //此时这个文件还不会被真正删除，直到这个进程关闭这个文件或进程结束，文件才被真正删除
  unlink("../test/test.txt");
  //即使下面运行出现了错误，程序崩溃，因为此时进程结束了，文件会被自动删除

  int size = read(fd, buf, sizeof(buf));
  write(STDOUT_FILENO, buf, size); 
  
  //下面代码会导致崩溃，崩溃后进程结束，文件会被真正删除
  test* t;
  t->a = 1;

  write(STDOUT_FILENO, buf, size);

  return 0;
}
