#include <iostream>
#include <math.h>

using namespace std;

class Distance
{
public:
  Distance(int x, int y) : x(x), y(y) 
  {
    cout << "x=" << x << endl;
    cout << "y=" << y << endl;
  }
  Distance operator-()
  {
    x = -x;
    y = -y;
    return Distance(x, y);
  }
  int x;
  int y;
};

int main()
{
  Distance d1(3, 4);
  Distance d2 = -d1;
  return 0;
}
