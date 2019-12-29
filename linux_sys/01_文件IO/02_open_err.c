#include <stdio.h> //perror
#include <fcntl.h> //open

int main()
{
  int fd;
#if 1
  //1.open不存在的文件
  fd = open("./test", O_RDONLY);
  if(fd < 0)
  {
    perror("open file error");//No such file or directory
  }
#endif
#if 0
  //2.write(只写或读写)只读的文件
  fd = open("./test.txt", O_WRONLY);//或O_RDWR
  if(fd < 0)
  {
    perror("open file error");//Permission denied
  }
#endif
#if 0
  //3.write（只写或读写）文件夹
  fd = open("./test", O_WRONLY);//或O_RDWR
  if(fd < 0)
  {
    perror("open file error");//Is a directory
  }
#endif

  return 0;
}
