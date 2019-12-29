#include <iostream>
#include <math.h>

using namespace std;

int sdk_init(int arg1,int arg2)
{
  cout << arg1 << "," << arg2 << endl;
  cout << "启动成功" << endl;
  return arg1+arg2;
}

//函数指针
void test1()
{
  int (*func)(int arg1,int arg2);
  func = sdk_init;
  func(4,5);
}

class Distance{
  private:
    double x;
    double y;
  public:
    Distance(int x, int y):x(x),y(y){}
    double displayDistance()
    {
      cout << sqrt(x*x+y*y) << endl;
    }

    double operator()(double h)
    {
      cout << "space is " << x*y*h << endl;
    }
};


//重载()运算符
void test2()
{
  Distance d1(3,4);
  d1.displayDistance();
  d1(10);
}

int main()
{
  // test1();
  test2();
  return 0;
}
