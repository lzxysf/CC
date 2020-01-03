#include <stdio.h>
#include <string.h>

typedef struct{
  int ID;
  int age;
  char* name;
  float score;
}Student;

void printScore(Student* stu)
{
  if(stu->score > 90)
  {
    printf("%s的成绩优秀\r\n");
  }
  else
  {
    printf("一般\r\n");
  }
}

void getnextyearage(Student* stu)
{
  printf("%s明年的年龄是%d岁\r\n", stu->name, stu->age+1);
}

int main()
{
  Student stu1;
  stu1.name = strdup("xiaoming");
  stu1.score = 96;
  stu1.age = 21;
  printScore(&stu1);
  getnextyearage(&stu1);
  return 0;
}
