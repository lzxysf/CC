#include <iostream>

/*
外观模式：
Facade模式为一组具有类似功能的类群，比如类库，子系统等等，提供一个一致的简单的界面。
这个一致的简单的界面被称作facade。
*/

using namespace std;

//子系统1
class SubSystem1
{
public:
  void func1()
  {
    cout << "invoke func1" << endl;
  }
};

//子系统2
class SubSystem2
{
public:
  void func2()
  {
    cout << "invoke func2" << endl;
  }
};

//子系统3
class SubSystem3
{
public:
  void func3()
  {
    cout << "invoke func3" << endl;
  }
};

//外观类
class Facade
{
public:
  void method()
  {
    s1.func1();
    s2.func2();
    s3.func3();
  }
private:
  SubSystem1 s1;
  SubSystem2 s2;
  SubSystem3 s3;
};

int main()
{
  Facade* f1 = new Facade();
  f1->method();
  delete f1;

  return 0;
}
