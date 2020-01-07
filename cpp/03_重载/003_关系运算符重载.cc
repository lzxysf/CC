#include <iostream>
#include <math.h>

using namespace std;

// #define INNER

class Distance
{
public:
#ifdef INNER
  bool operator>(const Distance& dis)
  {
    return (x*x+y*y) - (dis.x*dis.x+dis.y*dis.y) > 0 ? true : false;
  }
#endif
  int x;
  int y;
};

#ifndef INNER
  bool operator>(const Distance& dis1, const Distance& dis2)
  {
    return (dis1.x * dis1.x + dis1.y * dis1.y) - (dis2.x * dis2.x + dis2.y * dis2.y) > 0 ? true : false;
  }
#endif

int main()
{
  Distance d1;
  d1.x = 5;
  d1.y = 12;
  Distance d2;
  d2.x = 3;
  d2.y = 4;
  if(d1 > d2)
  {
    cout << "d1大于d2" << endl;
  }
  else
  {
    cout << "d1不大于d2" << endl;
  }
  return 0;
}
