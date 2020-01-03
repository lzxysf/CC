#include <iostream>

using namespace std;

/*
我们可以使用 static 关键字来把类成员定义为静态的。当我们声明类的成员为静态时，这意味着无论创建多少个类的对象，静态成员都只有一个副本。
类的静态成员变量在类中只是声明，必须在类外初始化（实质上是给静态成员变量分配内存），如果没有在类外初始化就会报错，而且只能在类外进行初始化，不能在类内进行初始化。
*/

class Box
{
public:
  Box(int w, int b, int h);
  ~Box();
  static int create_count;
  static int destroy_count;
  int width;
  int breadth;
  int height;
};

Box::Box(int w, int b, int h)
{
  create_count++;
  cout << "Box创建次数" << create_count << endl;
  width = w;
  breadth = b;
  height = h;
}
Box::~Box()
{
  destroy_count++;
  cout << "Box销毁次数" << destroy_count << endl;
}
int Box::create_count = 0;
int Box::destroy_count = 0;

int main()
{
  Box box1(1, 2, 3);
  Box box2(10, 20, 30);
  return 0;
}
