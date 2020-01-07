#include <iostream>

using namespace std;

/*
即()运算符的重载
operator函数应该作为类的成员函数，没有见过在类外不实现的。
*/

class Time
{
public:
  Time() {}
  Time(int hour, int minute) : hour(hour), minute(minute) {}
  
  void operator()(int minutes)
  {
    cout << "当前时间为" << minutes/60 << ":" << minutes%60 << endl;
  }
  
  void display()
  {
    cout << "当前时间为" << hour << ":" << minute << endl;
  }

private:
  int hour;
  int minute;
};

int main()
{
  Time time1(11, 59), time2;
  time1.display();
  
  time1(888);

  return 0;
}
