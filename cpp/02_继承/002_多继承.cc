#include <iostream>

using namespace std;

class A
{
public:
  int m_a;
};
class B
{
public:
  int m_b;
};
class C : public A, public B
{
public:
  int m_c;
};

int main()
{
  C c;
  c.m_a = 1;
  c.m_b = 2;
  c.m_c = 3;
  cout << "多继承测试" << endl;
  return 0;
}
