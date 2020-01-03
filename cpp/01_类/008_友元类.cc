#include <iostream>

using namespace std;

class Box
{
public:
  Box(int width, int breadth, int height) : width(width),breadth(breadth),height(height) {}
  friend void printVol(Box box);
  friend class BigBox;
private:
  int width;
  int breadth;
  int height;
};

void printVol(Box box)
{
  int vol = box.width * box.breadth * box.height;
  cout << "vol=" << vol << endl;
}

class BigBox
{
public:
  void expandWidth(Box &box)
  {
    box.width *= 2;
  }
  void expandBreadth(Box &box)
  {
    box.breadth *= 3;
  }
  void expandHeight(Box &box)
  {
    box.height *= 3;
  }
};

int main()
{
  Box box(10,5,8);
  printVol(box);

  BigBox bigbox;
  bigbox.expandWidth(box);
  bigbox.expandBreadth(box);
  bigbox.expandHeight(box);

  printVol(box);
  return 0;
}
