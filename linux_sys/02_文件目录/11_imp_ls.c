#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

int main(int argc, char* argv[])
{
  DIR* dp;
  struct dirent* elem;

  if(argc!=1 && argc!=2)
  {
    return -1;
  }
  if(argc==1)
  {
    dp = opendir("./");
  }
  else if(argc==2)
  {
    dp = opendir(argv[1]);
  }
  
  if(dp == NULL)
  {
    perror("opendir failed:");
  }
  while(elem=readdir(dp))
  {
    printf("%s\t", elem->d_name);
  }
  printf("\r\n");
  return 0;
}
