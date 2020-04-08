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

void setPeople(People people);
void showpeople(People people);
void setPeople2(People* people);
void showpeople2(People* people);

int main()
{
 People p1 = {0};
 setPeople(p1);
 showpeople(p1);

 People p2 = {0};
 setPeople2(&p2);
 showpeople2(&p2);
}

void setPeople(People people)
{
  people.name = strdup("lishifu");
  people.sex = true;
  people.address = strdup("shijiazhuang");
}
void showpeople(People people)
{
  printf("name:%s\r\n",people.name);
}

void setPeople2(People* people)
{
  people->name = strdup("lishifu");
  people->sex = true;
  people->address = strdup("shijiazhuang");
}
void showpeople2(People* people)
{
  printf("name:%s\r\n",people->name);
}
