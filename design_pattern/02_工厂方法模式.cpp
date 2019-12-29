#include <iostream>

using namespace std;

/*工厂方法模式*/
/*
工厂方法模式中每一个产品都对应一个工厂,会产生大量工厂类

简单工厂模式 + “开闭原则” = 工厂方法模式
优点：相比于简单工厂模式，系统的可扩展性变好，无需修改原类
缺点：类的个数增多，结构更复杂了
*/

//使用武器工厂制造大炮和飞机
//在工厂方法模式
class Arm
{
public:
  virtual void start() = 0;
};
class Cannon : public Arm
{
public:
  virtual void start()
  {
    cout << "大炮发射了" << endl;
  }
};
class Aircraft : public Arm
{
public:
  virtual void start()
  {
    cout << "飞机起飞了" << endl;
  }
};
class ArmFactory
{
public:
  virtual Arm* createArm() = 0;
};
class CannonFactory : public ArmFactory
{
public:
  virtual Arm* createArm()
  {
    return new Cannon;
  }
};
class AircraftFactory : public ArmFactory
{
public:
  virtual Arm* createArm()
  {
    return new Aircraft;
  }
};

int main()
{
  ArmFactory* cannon_factory = new CannonFactory;
  Arm* cannon = cannon_factory->createArm();
  cannon->start();

  return 0;
}
