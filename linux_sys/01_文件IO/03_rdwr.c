#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  int rd_fd, wr_fd;
  char buf[64];
  int count;

  rd_fd = open("../test/test.txt", O_RDONLY);
  if(rd_fd < 0)
  {
    perror("open input file error:");
  }

  wr_fd = open("../test/text.txt", O_WRONLY | O_CREAT | O_TRUNC, "0644");
  if(wr_fd < 0)
  {
    perror("open output file error:");
  }

  while(count = read(rd_fd, buf, sizeof(buf)))
  {
    write(wr_fd, buf, count);//不应为64，读多少写多少
  }

  close(rd_fd);
  close(wr_fd);

  return 0;
}
