#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> // truncate ftruncate

//int truncate(const char *path, off_t length);
//int ftruncate(int fd, off_t length);
//truncate或ftruncate是将文件大小改为参数length指定的大小
//如果原来的文件大小比参数length大，则超过的部分会被删去
//如果length比原来文件大小大，那么会扩展文件大小，扩展部分为均为0x00，但此时这部分还不是文件真实大小，掉电会丢失，在文件末尾处添加非0的有效数据可以使这一部分成为文件真实大小的一部分。
//查看文件的16进制形式使用命令“xxd 文件名”

int main()
{
#if 0
  int fp;
  fp = open("../test/test.txt", O_WRONLY);
  ftruncate(fp, 500);
  close(fp);
#endif
#if 1
  truncate("../test/test.txt",  1000);
#endif
  return 0;
}
