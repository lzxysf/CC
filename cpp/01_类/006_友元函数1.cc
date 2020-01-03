#include <iostream>

using namespace std;

/*
类的友元函数是定义在类外部，但有权访问类的所有私有（private）成员和保护（protected）成员。尽管友元函数的原型有在类的定义中出现过，但是友元函数并不是成员函数。
友元可以是一个函数，该函数被称为友元函数；友元也可以是一个类，该类被称为友元类，在这种情况下，整个类及其所有成员都是友元。
*/

class Box
{
public:
  Box(int w, int b, int h);
  friend void printBox(Box box);
private:
  int width;
  int breadth;
  int height;
};

Box::Box(int w, int b, int h) : width(w), breadth(b), height(h)
{
  cout << "构造函数" << endl;
}

void printBox(Box box)
{
  cout << "Box: width=" << box.width << ",breadth=" << box.breadth << ",height=" << box.height << endl;
}

int main()
{
  Box box(10,20,30);

  printBox(box);
  
  return 0;
}
