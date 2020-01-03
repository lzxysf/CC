#include <iostream>

using namespace std;

class Time
{
public:
  Time(int hour, int minute) : hour(hour), minute(minute) {}
  Time operator++ ()
  {
    minute++;
    if(minute==60)
    {
      hour++;
      minute = 0;
    }
    if(hour==24)
    {
      hour = 0;
    }
    return Time(hour, minute);
  }
  Time operator++(int)
  {
    Time temp(hour, minute);
    minute++;
    if(minute==60)
    {
      hour++;
      minute = 0;
    }
    if(hour==24)
    {
      hour = 0;
    }
    return temp;
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
  Time time(11, 59);
  time.display();

  Time time1 = ++time;
  time.display();
  time1.display();

  Time time2 = time++;
  time.display();
  time2.display();
  
  return 0;
}