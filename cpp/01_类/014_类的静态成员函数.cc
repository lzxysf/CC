#include <iostream>

using namespace std;

/*
静态成员函数没有this指针，只能访问静态成员（包含静态成员变量和静态成员函数）
普通成员函数包含this指针，可以访问类中的任意成员

静态成员函数可以在类中声明类外实现，也可以直接在类中实现。
*/

class Box
{
public:
  static int count;
  static int getCount()
  {
    cout << "调用次数" << count << endl;
    return count;
  }
  Box(int w, int b, int h);
  int width;
  int breadth;
  int height;
};

Box::Box(int w, int b, int h)
{
  count++;
  width = w;
  breadth = b;
  height = h;
}
int Box::count = 100;
// int Box::getCount()
// {
//   cout << "调用次数" << count << endl;
//   return count;
// }

int main()
{
  Box box1(1, 2, 3);
  Box box2(10, 20, 30);
  box1.getCount();
  return 0;
}
