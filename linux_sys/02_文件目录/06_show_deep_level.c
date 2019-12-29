#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void file_print(const char* path, int num)
{
  while(num--)
  {
    printf("\t");
  }
  printf("%s\r\n", path);
}

int deep_through(const char* path, int deep_level)
{
  char abs_path[1024];
  DIR* dp;
  struct dirent* elem;

  dp = opendir(path);
  if(dp == NULL)
  {
    perror("opendir failed:");
    return -1;
  }

  file_print(path, deep_level);
  deep_level++;

  while(elem=readdir(dp))
  {
    struct stat statbuf;

    if(strcmp(elem->d_name, ".")==0 || strcmp(elem->d_name, "..")==0)
    {
      continue;
    }
    sprintf(abs_path, "%s/%s", path, elem->d_name);
    lstat(abs_path, &statbuf);
    if(S_ISREG(statbuf.st_mode))
    {
      file_print(elem->d_name, deep_level);
    }
    else if(S_ISDIR(statbuf.st_mode))
    {
      deep_through(abs_path, deep_level);
    }
  }
}

int main()
{
  char buf[256];
  const char* path = "../../ds&algorithm";
  chdir(path);
  char* ret = getcwd(buf, 256);
  if(ret == NULL)
  {
    return -1;
  }
  deep_through(buf, 0);
  return 0;
}
