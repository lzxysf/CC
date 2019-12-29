#include <stdio.h>
#include <unistd.h>
#include <time.h>
#if 0
int main()
{
  /*得出结论字符数组的 首地址可以用 arrgs ，&arrgs,来表示并且它们相等，
  但是指针&str和str是不同的,当遇到字符串指针时候要注意处理方式;*/
  char* str = "hello";
  char args[] = "hello";
  printf("%p,%p\r\n",str,&str);
  printf("%p,%p\r\n",args,&args);
  return 0;
}
int main()
{
  /*指针p可以指向数组a的地址，但是a不能指向指针p的地址，
  因为数组是const的指针，不可改变，即char a[]实际上是char const *a; */
  char a[] = "hello";
  char* p;
  p = a;
  printf("%p,%p",a,p);//相等的
}
int main()
{
  /*字符串可以直接输出*/
  char* p = "hello";
  printf(p);
}
int main()
{
    /*返回值为0说明存在文件或文件夹，为-1说明不存在*/
    int ret = access("/usr/local",0);
    printf("%d",ret);
}
#endif

struct callinfo{
  char udn[50];
  int cid;
  time_t t;
};

int get_timestamp();

int main()
{
  printf("%d\n", get_timestamp());
  return 0;
}

int get_timestamp()
{
  time_t t = time(NULL);
  int tt = time(&t);
  return tt;
}