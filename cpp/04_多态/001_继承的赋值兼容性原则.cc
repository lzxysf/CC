#include <iostream>

using namespace std;

/*
继承的兼容性原则包括：
1、父类指针可以指向子类对象
2、父类引用可以指向子类对象
3、子类对象可以直接初始化父类对象

继承的类型兼容性原则是多态性的重要基础之一

父类的引用指向子类，相当于子类对象向上转型了，此时只能调用父类的方法，无法调用子类的方法。
*/

class Parent
{
public:
  void print()
  {
    cout << "i am parent" << endl;
  }
};

class Child : public Parent
{
public:
  void print()
  {
    cout << "i am child" << endl;
  }
};

int main() {
  Parent p;
  p.print();//i am parent

  Child c;
  c.print(); //i am child
  
  Parent pp = (Parent)c;
  pp.print(); //i am parent

  Parent *base = NULL;
  base = &p;
  base->print();//i am parent

  base = &c;
  base->print();//i am parent
  
  return 0;
}