#include <iostream>
#include <math.h>

using namespace std;

class Distance
{
public:
  double operator-(const Distance& dis)
  {
    return sqrt(x*x+y*y) - sqrt(dis.x*dis.x+dis.y*dis.y);
  }
  int x;
  int y;
};

int main()
{
  Distance d1;
  d1.x = 3;
  d1.y = 4;
  Distance d2;
  d2.x = 5;
  d2.y = 12;
  double ret = d1 - d2;
  cout << ret << endl;
  return 0;
}
