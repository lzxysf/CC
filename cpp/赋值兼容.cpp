#include <iostream>

using namespace std;

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