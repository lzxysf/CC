#include <iostream>


//将一个类的接口转换成客户希望的另外一个接口，使得原本由于接口不兼容而不能一起工作的那些类可以一起工作
using namespace std;

//目标类，客户希望调用的类
class Target
{
public:
  virtual void request() = 0;
};

//被适配类，实际功能实现的类
class Adaptee
{
public:
 void specificrequest()
 {
   cout << "invoke specific request" << endl;
 }
};

//适配器类，用于接口转换
//Target类是接口继承，Adaptee类是实现继承
class Adapter : public Target
{
public:
  virtual void request()
  {
    adaptee.specificrequest();
  }
private:
  Adaptee adaptee;
};

int main()
{
  Target* target = new Adapter();
  target->request();
  delete target;

  return 0;
}
