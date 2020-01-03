#include <iostream>

using namespace std;

class Box
{
public:
  Box(int w, int b, int h);
  void printVol();
  int width;
  int breadth;
  int height;
};
Box::Box(int w, int b, int h)
{
  this->width = w;
  this->breadth = b;
  this->height = h;
}
void Box::printVol()
{
  cout << "Box体积为" << width * breadth * height << endl;
}

int main()
{
  Box box1(10, 20, 30);
  box1.printVol();

  Box *box2 = new Box(1, 5, 10);
  box2->printVol();

  Box *box3 = &box1;
  box3->printVol();

  return 0;
}
