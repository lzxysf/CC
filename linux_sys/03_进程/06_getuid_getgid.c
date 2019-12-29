#include <stdio.h>
#include <unistd.h>

//getuid() :  函数返回一个调用程序的真实用户ID。表明当前运行位置程序的执行者。
//geteuid(): 函数返回返回一个有效用户的ID。（EUID）是你最初执行程序时所用的ID，该ID是程序的所有者。   
//getgid(): 获取当前进程使用用户组ID
//getegid: 获取当前进程有效用户组ID

int main()
{
  int uid = getuid();
  int euid = geteuid();
  int gid = getgid();
  int egid = getegid();
  printf("uid=%d, euid=%d, gid=%d, egid=%d\r\n", uid, euid, gid, egid);
  return 0;
}
