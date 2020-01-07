#include <iostream>

using namespace std;

// #define INNER

/*
类之间可以直接通过=赋值,一般无需重载=运算符
但如果类对象的复制需要开辟堆内存空间或者需要做其他改造，那么就需要重载=运算符
*/

class Complex
{
public:
  Complex(int a, int b) : a(a), b(b) {}
#ifdef INNER
  Complex& operator=(const Complex &another)
  {
    cout << "=运算符重载" << endl;
    a = another.a;
    b = another.b;
    return *this;
  }
#else
  friend Complex& operator=(Complex &c, const Complex &another);
#endif
  void display()
  {
    cout << "当前复数为:" << a << "+" << b << "i" << endl;
  }
private:
  int a;
  int b;
};

#ifndef INNER
Complex& operator=(Complex &c, const Complex &another)
{
  c.a = another.a;
  c.b = another.b;
  return c;
}
#endif

int main()
{
  Complex c1(5, 6), c2(4, 15);
  c1.display();
  c2.display();
  
  cout << "-------------------" << endl;
  c2 = c1;
  c1.display();
  c2.display();

  return 0;
}
