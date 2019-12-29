#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  int num = atoi(argv[1]);
  for(int i = 0; i < num; i++)
  {
    printf("%s\r\n", argv[2]);
    // sleep(1);
  }
  return 0;
}