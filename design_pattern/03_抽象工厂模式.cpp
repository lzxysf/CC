#include <iostream>

/*抽象工厂模式*/
/*抽象工厂模式中，同一个产品族的产品在一个工厂*/
/*
适用于：
  系统中有多个产品族（本例中的美国武器和中国武器）
  系统的产品等级结构稳定（本例中中国武器分为大炮类和战机类，不再添加新的武器）
*/
using namespace std;

class Arm
{
public:
  virtual void start() = 0;
};
class Cannon : public Arm
{
public:
  virtual void start() = 0;
};
class Aircraft : public Arm
{
public:
  virtual void start() = 0;
};
class AmeriCannon : public Cannon
{
public:
  virtual void start()
  {
    cout << "Ameri cannon start" << endl;
  }
};
class AmeriAircraft : public Aircraft
{
public:
  virtual void start()
  {
    cout << "Ameri aircraft fly" << endl;
  }
};
class ChinaCannon : public Cannon
{
public:
  virtual void start()
  {
    cout << "China cannon start" << endl;
  }
};
class ChinaAircraft : public Aircraft
{
public:
  virtual void start()
  {
    cout << "China aircraft fly" << endl;
  }
};
class Factory
{
public:
  virtual Cannon* createCannon() = 0;
  virtual Aircraft* createAircraft() = 0;
};
class AmeriFactory : public Factory
{
public:
  virtual Cannon* createCannon()
  {
    return new AmeriCannon;
  }
  virtual Aircraft* createAircraft()
  {
    return new AmeriAircraft;
  }
};
class ChinaFactory : public Factory
{
public:
  virtual Cannon* createCannon()
  {
    return new ChinaCannon;
  }
  virtual Aircraft* createAircraft()
  {
    return new ChinaAircraft;
  }
};

int main()
{
  //制造一个中国战机、中国大炮
  Factory* cn_factory = new ChinaFactory;
  Aircraft* cn_aircraft = cn_factory->createAircraft();
  Cannon* cn_cannon = cn_factory->createCannon();
  cn_aircraft->start();
  cn_cannon->start();

  //制造一个美国大炮、美国战机
  Factory* us_factory = new AmeriFactory;
  Cannon* us_cannon = us_factory->createCannon();
  Aircraft* us_aircraft = us_factory->createAircraft();
  us_cannon->start();
  us_aircraft->start();
  return 0;
}
