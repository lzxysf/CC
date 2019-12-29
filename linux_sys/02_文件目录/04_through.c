#include <stdio.h>
#include <unistd.h>
#include <dirent.h> // DIR opendir readdir

//遍历
void through(const char* path)
{
  DIR* dp;
  struct dirent* elem;
  
  dp = opendir(path);
  if(dp == NULL)
  {
    return;
  }

  while(elem=readdir(dp))
  {
    printf("%s\r\n", elem->d_name);
  }
}

int main()
{
  const char* root = "../../ds&algorithm";
  through(root);
  return 0;
}
