#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main()
{
  struct winsize size;
  int is_atty = isatty(STDOUT_FILENO);
  if(!is_atty)
  {
    return 1;
  }
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
  printf("%d %d", size.ws_row, size.ws_col);

  return 0;
}
