#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

struct _People{
  char* name;
  bool sex;
  char* address;
};
typedef struct _People People;

int main()
{
  People* p = NULL;
  p = (People*)malloc(sizeof(People));
  p->name = strdup("lishuaifu");
  p->sex = true;
  p->address = strdup("shijiazhuang");
  printf("name:%s, sex:%d，address:%s\r\n",p->name,p->sex,p->address);
  if(p != NULL)
  {
    if(p->name != NULL)
    {
      free(p->name);
      p->name = NULL;
    }
    if(p->address != NULL)
    {
      free(p->address);
      p->address = NULL;
    }
    free(p);
    p = NULL;
  }
}
