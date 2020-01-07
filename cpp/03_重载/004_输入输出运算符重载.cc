#include <iostream>

using namespace std;

/*
输入运算符重载的时候，distance类对象必须传引用，否则修改的是子函数的对象副本成员变量

输入输出运算符重载一般必须在类外部编写，不适用类成员函数的方式。
*/

class Distance
{
public:
  Distance() : x(0), y(0) {}
  Distance(int x,int y) : x(x), y(y) {}
  friend ostream &operator<<(ostream &output, Distance& d)
  {
    output << d.x << " " << d.y << endl;
    return output;
  }
  friend istream &operator>>(istream &input, Distance& d)
  {
    input >> d.x >> d.y;
    return input;
  }
private:
  int x;
  int y;
};

int main()
{
  Distance d1(10,20), d2(20,30), d3;
  cout << d1 << d2;
  cin >> d3;
  cout << d3;
  return 0;
}
