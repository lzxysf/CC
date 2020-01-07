#include <iostream>

using namespace std;

/*
1.纯虚函数在父类是没有定义的，只有声明，并且子类必须要重写纯虚函数否则会报错。
2.一个类，只要有纯虚函数就称为抽象类，抽象类不能实例化，只有在其子类中实例化了纯虚函数，子类才能实例化。
*/

class Animal
{
public:
	virtual void eat() = 0;
	virtual void sleep() = 0;
};

class Dog : public Animal
{
public:
    void eat()
	{
      cout << "狗在吃骨头" << endl;
    }
	void sleep()
	{
		cout << "狗在狗窝睡觉" << endl;
	}
};

class Cow : public Animal
{
 public:
    void eat()
	{
      cout << "牛在吃草" << endl;
    }
	void sleep()
	{
		cout << "牛在牛棚睡觉" << endl;
	}
};

int main()
{
  Animal *base = NULL;
  base = new Dog();
  base->eat();      //狗在吃骨头
  base->sleep();	//狗在狗窝睡觉

  base = new Cow();
  base->eat();      //牛在吃草
  base->sleep();	//牛在牛棚睡觉
  
  return 0;
}
