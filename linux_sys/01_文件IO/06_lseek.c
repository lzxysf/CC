#include <stdio.h>
#include <unistd.h> //lseek
#include <fcntl.h>

//off_t lseek(int fd, off_t offset, int whence);
//lseek()函数会重新定位被打开文件的位移量，根据参数offset以及whence的组合来决定
//文件必须是打开的
//offset为正则向文件末尾移动，为负数则向文件头部移动
//SEEK_SET: 从文件头部开始偏移offset个字节
//SEEK_CUR：从文件当前读写的指针位置开始偏移offset个字节
//SEEK_END：从文件末尾偏移offset个字节
//返回值：成功则返回从开头计数的位置，失败返回-1

int main()
{
  int fd;
  int off;
  int buf[64];
  int n;

  fd = open("../test/test.txt", O_RDWR);
  if(fd<0)
  {
    perror("open file error");
  }
#if 0
  //根据lseek定位文件大小
  off = lseek(fd, 0, SEEK_END);
  printf("file size is %d\r\n", off);
#endif
#if 0
  off = lseek(fd, 100, SEEK_SET);
  while(n = read(fd, buf, sizeof(buf)))
  {
    write(STDOUT_FILENO, buf, n);
  }
#endif
#if 1
  //lseek扩展文件,打开的文件必须是可写的，必须有一个实际的IO操作才能完成扩展
  off = lseek(fd, 999, SEEK_SET);
  write(fd, "\0", 1);//实际的IO操作，写入任何东西都可以
#endif
  return 0;
}
