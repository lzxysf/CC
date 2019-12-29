#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> //DIR opendir readdir
#include <sys/stat.h> //lstat

void deep_through(const char* path)
{
  DIR* dp;
  struct dirent* elem;
  char abs_path[1024];

  dp = opendir(path);
  if(dp == NULL)
  {
    perror("opendir failed:");
    return;
  }

  printf("%s\r\n", path);

  while(elem=readdir(dp))
  {
    if(strcmp(elem->d_name, ".")==0 || strcmp(elem->d_name, "..")==0)
    {
      continue;
    }
    sprintf(abs_path, "%s/%s", path, elem->d_name);//绝对路径
    struct stat statbuf;//文件状态描述结构体stat
    lstat(abs_path, &statbuf);//获取文件状态结构体
    if(S_ISREG(statbuf.st_mode)) //普通文件
    {
      printf("%s\r\n", elem->d_name);
    }
    else if(S_ISDIR(statbuf.st_mode)) //目录文件
    {
      deep_through(abs_path);
    }
  }
}

int main()
{
  const char* path = "../../ds&algorithm";
  deep_through(path);
  return 0;
}
