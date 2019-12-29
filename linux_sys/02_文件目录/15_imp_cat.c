#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// ./a.out test1 > test2 或者 ./a.out test1
int main(int argc, char* argv[])
{
  if(argc == 4 && strcmp(argv[2], ">")==0)
  {
    int fd1 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, "0644");
    if(dup2(fd1, STDOUT_FILENO)==-1)
    {
      perror("dup2 error:");
      return -1;
    }
    close(fd1);
  }

  int fd = open(argv[1], O_RDONLY);
  char buf[64];
  int n;
  while(n = read(fd, buf, sizeof(buf)))
  {
    // write(STDOUT_FILENO, buf, n);
    printf("%s", buf);
  }
  close(fd);

  return 0;
}
