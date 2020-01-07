#include <iostream>

using namespace std;

class Time
{
public:
  Time() {}
  Time(int hour, int minute) : hour(hour), minute(minute) {}
  
  Time operator()(int a, int b, int c)
  {
    Time temp;
    temp.hour = a + b;
    temp.minute = a + c;
    return temp;
  }

  void display()
  {
    cout << "当前时间为" << hour << ":" << minute << endl;
  }

  int hour;
  int minute;
};

int main()
{
  Time time1(11, 59), time2;
  time1.display();
  
  time2 = time1(1, 2, 3);
  time1.display();
  time2.display();

  return 0;
}
