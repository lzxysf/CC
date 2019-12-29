#include <iostream>

using namespace std;

/*简单工厂模式的优缺点*/
/*
简单工厂模式中，所有的产品都在一个工厂生产
优点：
1.实现了对象创建和使用的分离
2.实现简单，十分适合小规模产品数量
缺点：
1.工厂类太庞大，且随着产品的增加会越来越庞大难以维护，违反了“单一职责原则”
2.每次增加产品都要修改工厂类，违反了“开闭原则”
*/

//使用武器工厂制造大炮和飞机
//武器基类
class Arm
{
public:
  virtual void start() = 0;
};
//大炮类
class Cannon : public Arm
{
public:
  virtual void start()
  {
    cout << "大炮发射了" << endl;
  }
};
//飞机类
class Aircraft : public Arm
{
public:
  virtual void start()
  {
    cout << "飞机起飞了" << endl;
  }
};
//添加手枪类
class Gun : public Arm
{
public:
  virtual void start()
  {
    cout << "手枪发射子弹了" << endl;
  }
};

class ArmFactory
{
public:
  Arm* createArm(string kind)
  {
    Arm* arm = NULL;
    if(kind.compare("aircraft")==0)
    {
      arm = new Aircraft;
    }
    else if(kind.compare("cannon")==0)
    {
      arm = new Cannon;
    }
    //添加制造手枪产品的分支
    else if(kind.compare("gun")==0)
    {
      arm = new Gun;
    }
    return arm;
  }
};

int main()
{
  ArmFactory* factory = new ArmFactory();
  Arm* cannon = factory->createArm("cannon");
  cannon->start();

  return 0;
}
