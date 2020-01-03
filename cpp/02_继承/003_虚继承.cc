#include <iostream>

using namespace std;

/*
http://c.biancheng.net/view/2280.html

多继承很容易产生命名冲突，最典型的就是菱形继承
菱形继承：两个子类继承同一个父类，而又有子类多继承这两个子类
          A
        /   \
       B     C
        \   /
          D 
这样在D中就会有两份A中同名成员的拷贝，这样就会产生冗余。
此时使用虚继承，即B和C都虚继承自A，这样D在继承B和C后就只有一份A中成员的副本。

虚继承的目的是让某个类做出声明，承诺愿意共享它的基类。其中，这个被共享的基类就称为虚基类（Virtual Base Class），本例中的 A 就是一个虚基类。在这种机制下，不论虚基类在继承体系中出现了多少次，在派生类中都只包含一份虚基类的成员。
*/

#if 0
//不使用虚继承
class A
{
public:
  int m_a;
};
class B : public A
{
public:
  int m_b;
};
class C : public A
{
public:
 int m_c;
};
class D : public B, public C
{
public:
  int m_d;
  void seta(int a)
  {
    // m_a = a; //“m_a = a”是错误的，此时m_a有两个副本，应该指定B::m_a或C::m_a
    B::m_a = a;
  }
};
#else
//使用了虚继承
class A
{
public:
  int m_a;
};
class B : virtual public A
{
public:
  int m_b;
};
class C : virtual public A
{
public:
 int m_c;
};
class D : public B, public C
{
public:
  int m_d;
  void seta(int a)
  {
    m_a = a; //“m_a = a”是正确的，全局只有一个m_a的副本
  }
};
#endif

int main()
{
  D d;
  d.seta(10);
  return 0;
}
