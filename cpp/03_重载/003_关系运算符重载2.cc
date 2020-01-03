#include <iostream>
#include <math.h>

using namespace std;

/*
此处重载函数需要加friend，因为需要在类外部访问类的私有成员。

友元函数虽然写在了类的内部，但友元函数不是类的成员函数，因此是在类外部访问类的私有成员。
*/

class Distance
{
public:
  friend bool operator>(const Distance& dis1, const Distance& dis2)
  {
      return (dis1.x * dis1.x + dis1.y * dis1.y) - (dis2.x * dis2.x + dis2.y * dis2.y) > 0 ? true : false;
  }
  void setx(int x){this->x = x;}
  void sety(int y){this->y = y;}
private:
  int x;
  int y;
};

int main()
{
  Distance d1,d2;
  d1.setx(5);
  d1.sety(12);
  d2.setx(3);
  d2.sety(4);
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
