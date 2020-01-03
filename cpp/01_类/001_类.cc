#include <iostream>

using namespace std;

class Box
{
public:
  int width;
  int breadth;
  int height;
  int getVolume()
  {
    return width * breadth * height;
  }
};

int main()
{
  Box box1;
  box1.width = 10;
  box1.breadth = 20;
  box1.height = 30;
  int vol = box1.getVolume();
  cout << "体积为" << vol << endl;
  return 0;
}
