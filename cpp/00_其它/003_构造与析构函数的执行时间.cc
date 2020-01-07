#include <iostream>
#include <stdio.h>

using namespace std;

/*
在栈上的未析构的类对象会在类对象离开作用域后自动调用析构函数
在堆上的未析构的类对象需要手动调用delete才会执行析构函数
*/

class Line
{
public:
  int length;
  Line(int length);
  ~Line();
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

//1.全局变量定义的类对象
//构造函数会在main函数之前执行
Line line1(1);
Line *line2 = new Line(2);

int main()
{
  {
	  //2.局部变量定义的类对象
	  //构造函数会在类对象定义时执行，析构函数会在离开作用域范围时执行
	  Line line3(3);
  }
	
  delete line2;
  
  return 0;
}
