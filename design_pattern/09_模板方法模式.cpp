#include <iostream>

//模版方法模式（Template Method Pattern）定义一个操作中算法的骨架，而将一些步骤延迟到子类中。
//模板方法模式使得子类可以不改变一个算法的结构即可重定义该算法的某些特定步骤。
//模板方法模式可实现一种反向控制结构，通过子类覆盖父类的钩子方法来决定某一特定步骤是否需要执行

using namespace std;

class AbstractClass
{
public:
  virtual void A()
  {
    cout << "A" << endl;
  }
  virtual void B() = 0;
  virtual void C() = 0;
  virtual bool judge()
  {
    return true;
  }
  void work()
  {
    A();
    B();
    if(judge())
    {
      C();
    }
  }
};

class ConcreteClass : public AbstractClass
{
public:
  virtual void B()
  {
    cout << "B" << endl;
  }
  virtual void C()
  {
    cout << "C" << endl;
  }
  virtual bool judge()
  {
    return false;
  }
};

int main()
{
  AbstractClass* abc = new ConcreteClass();
  abc->work();
  delete abc;
}
