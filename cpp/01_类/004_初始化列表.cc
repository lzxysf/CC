#include <iostream>
#include <stdio.h>

using namespace std;

/*
可以使用初始化列表来初始化类成员，
使用初始化列表初始化类成员时，是按照类成员在类中声明的顺序初始化的，而不是按照出现在初始化列表中的顺序，
本例中，是按照bold,length来初始化的，而不是按照length,bold顺序初始化的
*/

class Line
{
public:
  Line(int length, int bold);  //构造函数
  ~Line();                    //析构函数
private:
  int bold;
  int length;
};

Line::Line(int length, int bold) : length(length), bold(bold)
{
  cout << "这是构造函数, len is " << Line::length << ", bold is " << Line::bold << endl;
}

Line::~Line()
{
  cout << "这是析构函数" << endl;
}

int main()
{
  Line line(100, 20);

  getchar();

  return 0;
}
