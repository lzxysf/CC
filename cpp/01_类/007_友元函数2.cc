#include <iostream>

using namespace std;

/*
因为友元函数没有this指针，则参数有以下情况： 
要访问非static成员时，需要对象做参数；
要访问static成员或全局变量时，则不需要对象做参数。
*/

class Box
{
public:
  Box(int w, int b, int h);
  static int shape;
  friend void printBox();
private:
  int width;
  int breadth;
  int height;
};

Box::Box(int w, int b, int h) : width(w), breadth(b), height(h)
{
  cout << "构造函数" << endl;
}

int Box::shape = 100;

void printBox()
{
  cout << "Box: shape=" << Box::shape << endl;
}

int main()
{
  Box box(10,20,30);

  printBox();
  
  return 0;
}
