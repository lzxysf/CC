#include <iostream>
#include <math.h>

using namespace std;

#define INNER

/*
运算符重载可分为，成员函数方式重载、类外部重载。

本例中类外部重载函数需要加friend，因为需要在类外部访问类的私有成员。

友元函数虽然写在了类的内部，但友元函数不是类的成员函数，因此是在类外部访问类的私有成员。
*/

class Distance
{
public:
  Distance(int x, int y) : x(x), y(y) {}
#ifdef INNER
  Distance operator-()
  {
    Distance temp(-x, -y);
    return temp;
  }
#else
  friend Distance operator-(const Distance& d);
#endif
  void dispaly()
  {
    cout << "x=" << x << " y=" << y << endl;
  }
private:
  int x;
  int y;
};

#ifndef INNER
Distance operator-(const Distance& d)
{
  Distance temp(-d.x, -d.y);
  return temp;
}
#endif

int main()
{
  Distance d1(3, 4);
  Distance d2 = -d1;
  // Distance d2 = -(-d1);
  d2.dispaly();
  return 0;
}
