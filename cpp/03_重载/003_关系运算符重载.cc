#include <iostream>
#include <math.h>

using namespace std;

class Distance
{
public:
  bool operator>(const Distance& dis)
  {
    return (x*x+y*y) - (dis.x*dis.x+dis.y*dis.y) > 0 ? true : false;
  }
  int x;
  int y;
};

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
