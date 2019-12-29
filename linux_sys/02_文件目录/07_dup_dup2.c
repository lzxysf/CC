#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/*
#include <unistd.h>
int dup(int oldfd);
int dup2(int oldfd,int newfd);
功能:文件描述符配置
dup:用来复制参数oldfd所指的文件描述符。
当复制成功时，返回最小的尚未被使用过的文件描述符，若有错误则返回-1。
返回的新文件描述符和参数oldfd指向同一个文件，这两个描述符共享同一个数据结构，共享所有的锁定，读写指针和各项权限或标志位。
dup2:dup2与dup区别是,dup2可以用参数newfd指定新文件描述符的数值。若参数newfd已经被程序使用，则系统就会将newfd所指的文件关闭，若newfd等于oldfd，则返回newfd,而不关闭newfd所指的文件。dup2所复制的文件描述符与原来的文件描述符共享各种文件状态。共享所有的锁定，读写位置和各项权限或flags等。
使用dup/dup2给一个文件制定了两个描述符的时候，一个文件描述符关闭，依然能使用另一个文件描述符对该文件读写。
*/

int main()
{
  int fd;
  fd = open("../test/test.txt", O_RDWR);
  if(fd < 0)
  {
    perror("open file error");
    return -1;
  }
#if 0
  int newfd = dup(fd);
  printf("oldfd:%d,newfd:%d\r\n", fd, newfd);//oldfd:3,newfd:4
  lseek(fd, 0, SEEK_END);
  close(fd);
  write(newfd, "test", 4);
  close(newfd);
#endif
#if 0
  dup2(STDOUT_FILENO, 4);
  printf("oldfd:%d,newfd:%d\r\n", STDOUT_FILENO, 4);//oldfd:1,newfd:4
  write(4, "hello world", 11);
#endif
#if 0
  dup2(STDOUT_FILENO, fd);//fd这个文件描述符已经被使用了，此时关闭fd指向的文件，并将fd指向STDOUT_FILENO
  write(fd, "hello world", 11);
#endif
#if 1
  dup2(fd, STDOUT_FILENO);//此时STDOUT_FILENO也会指向fd所指向的文件(fd会覆盖STDOUT_FILENO),此时向标准输出设备输出的字符会输出到fd指向到文件test.txt中
  // write(STDOUT_FILENO, "hello world", 11);
  printf("hello world");
#endif
  return 0;
}
