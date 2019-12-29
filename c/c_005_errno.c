#include <stdio.h> //perror
#include <fcntl.h>
#include <errno.h> //errno
#include <string.h> //strerror

int main()
{
  int fd = open("./test.txt", O_RDONLY);
  if(fd==-1)
  {
    perror("open file error");

    printf("错误码为:%d\r\n", errno);

    char* str = strerror(errno);
    printf("%s\r\n", str);
  }
  return 0;
}
