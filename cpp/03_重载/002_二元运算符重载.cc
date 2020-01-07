#include <iostream>
#include <math.h>

using namespace std;

// #define INNER

class Distance
{
public:
#ifdef INNER
  Distance operator-(const Distance& dis)
  {
    Distance temp;
    temp.x = x - dis.x;
    temp.y = y - dis.y;
    return temp;
  }
#endif
  void display()
  {
    cout << "x=" << x << " y=" << y << endl;
  }
  int x;
  int y;
};

#ifndef INNER
  Distance operator-(const Distance& dis1, const Distance& dis2)
  {
    Distance temp;
    temp.x = dis1.x-dis2.x;
    temp.y = dis1.y-dis2.y;
    return temp;
  }
#endif

int main()
{
  Distance d1;
  d1.x = 3;
  d1.y = 4;
  Distance d2;
  d2.x = 5;
  d2.y = 12;
  Distance d3 = d2 - d1;
  // Distance d3 = d2 - d1 - d1;
  d3.display();
  return 0;
}
