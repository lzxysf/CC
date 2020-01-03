#include <iostream>

using namespace std;

/*
类的拷贝构造函数

拷贝构造函数是一种特殊的构造函数，它在创建对象时，是使用同一类中之前创建的对象来初始化新创建的对象。

如果在类中没有定义拷贝构造函数，编译器会自行定义一个。

如果类带有指针变量，并有动态内存分配，则它必须有一个拷贝构造函数。
*/

class Line
{
public:
  int getLength();
  Line(int len);
  Line(const Line &obj);
  ~Line();
private:
  int *ptr;
};

int Line::getLength()
{
  return *ptr;
}

Line::Line(int len)
{
  cout << "调用构造函数" << endl;
  ptr = new int;
  *ptr = len;
}

Line::Line(const Line &obj)
{
  cout << "调用拷贝构造函数" << endl;
  ptr = new int;
  *ptr = *(obj.ptr);
}

Line::~Line()
{
  cout << "调用析构函数" << endl;
}

int main()
{
  Line line1(100);

  Line line2(line1);

  int len = line2.getLength();

  cout << "len is " << len << endl;

  return 0;
}
