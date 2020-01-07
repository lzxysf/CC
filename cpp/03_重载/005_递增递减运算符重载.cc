#include <iostream>

using namespace std;

// #define INNER

/*
++a返回的是变量a的引用，可以作为左值，因为是引用，所以返回值要用引用的形式
a++返回的是数值，不可以作为左值

Time& operator++()    是重载前++，即++a这样的运算符
Time operator++(int)  是重载后++，即a++这样的运算符,其中int是亚元，起到了占位和标识的作用
*/

class Complex
{
public:
  Complex(int a, int b) : a(a), b(b) {}
#ifdef INNER
  Complex& operator++ ()
  {
    a++;
    b++;
    return *this;
  }
  Complex operator++(int)
  {
    Complex temp(a, b);
    a++;
    b++;
    return temp;
  }
#else
  friend Complex& operator++(Complex &c);
  friend Complex operator++(Complex &c, int);
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
Complex& operator++(Complex &c)
{
  c.a++;
  c.b++;
  return c;
}
Complex operator++(Complex &c, int)
{
  Complex temp(c.a, c.b);
  c.a++;
  c.b++;
  return temp;
}
#endif

int main()
{
  Complex c1(2, 5), c2(8, 13);
  c1.display();
  c2.display();

  cout << "---------------" << endl;
  Complex c3 = ++c1;
  c1.display();
  c3.display();

  cout << "---------------" << endl;
  ++c1 = c2;
  c1.display();

  cout << "---------------" << endl;
  Complex c4 = c2++;
  c2.display();
  c4.display();
  
  return 0;
}
