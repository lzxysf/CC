#include <stdio.h>
#include <stdlib.h> //exit
#include <unistd.h> //open read write
#include <fcntl.h> //O_RDONLY O_WRONLY O_RDWR
#include <errno.h> //errno

/*
man 2 open可以查看open函数man手册
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);

必选项:以下三个常数中必须指定一个,且仅允许指定一个。
O_RDONLY    只读打开
O_WRONLY    只写打开
O_RDWR      可读可写打开
以下可选项可以同时指定0个或多个,和必选项按位或起来作为flags参数。
O_APPEND    表示追加。如果文件已有内容,这次打开文件所写的数据附加到文件的末尾而不覆盖原来的内容。
O_CREAT     若此文件不存在则创建它。使用此选项时需要提供第三个参数mode ,表示该文件的访问权限。
O_EXCL      如果同时指定了O_CREAT,并且文件已存在,则出错返回。
O_TRUNC     如果文件已存在,并且以只写或可读可写方式打开,则将其长度截断(Truncate)为0字节。
O_NONBLOCK  对于设备文件,以O_NONBLOCK 方式打开可以做非阻塞I/O(Nonblock I/O)。
*/

int main()
{
  char buf[64];
  int size;

  int fd = open("./test.txt", O_RDONLY | O_CREAT);
  if(fd == -1)
  {
    printf("open file error, errno is %d\r\n", errno);
    exit(1);
  }

  while((size = read(fd, buf, sizeof(buf)) > 0))
  {
      write(STDOUT_FILENO, buf, size);
  }

  printf("fd=%d\r\n", fd);
  return 0;
}
