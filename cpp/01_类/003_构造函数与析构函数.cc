#include <iostream>
#include <stdio.h>

using namespace std;

/*
不主动调用析构函数的话，析构函数会在程序结束时执行。
*/

class Line
{
public:
  int length;
  Line(int length);       //构造函数
  ~Line();                //析构函数
};

Line::Line(int length)
{
  length = length;
  cout << "这是构造函数, len is " << length << endl;
}

Line::~Line()
{
  cout << "这是析构函数" << endl;
}

int main()
{
  Line line(100);

  getchar();

  return 0;
}
