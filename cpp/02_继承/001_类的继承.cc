#include <iostream>

using namespace std;

/*
1.父类-子类，基类-派生类

2.继承类型
公有继承————当一个类派生自公有基类时，基类的公有成员也是派生类的公有成员，基类的保护成员也是派生类的保护成员，基类的私有成员不能直接被派生类访问，但是可以通过调用基类的公有和保护成员来访问。
保护继承————当一个类派生自保护基类时，基类的公有和保护成员将成为派生类的保护成员。
私有继承————当一个类派生自私有基类时，基类的公有和保护成员将成为派生类的私有成员。

3.子类继承父类，不继承的东西有
父类的构造函数、拷贝构造函数、析构函数
父类的友元函数
父类的重载运算符

4.父类有自己的有参构造函数没有无参构造函数时，子类必须在初始化列表显式调用父类的有参构造函数，否则会报错
*/

class Animal
{
public:
  Animal(char* name, int age, int type)
  {
    this->name = name;
    this->age = age;
    this->type = type;
    cout << "创建了一个动物" << "name=" << name << ",age=" << age << ",type=" << type << endl;
  }
  void eat()
  {
    cout << "动物在吃饭" << endl;
  }
  void run()
  {
    cout << "动物在奔跑" << endl;
  }
private:
  char* name;
  int age;
  int type;
};

class Dog : public Animal
{
public:
  Dog(char* name, int age, int type, int id) : Animal(name, age, type), id(id) {}
  void eat()
  {
    cout << "狗在吃骨头" << endl;
  }
  void run()
  {
    cout << "狗在奔跑" << endl;
  }
private:
  int id;
};

int main()
{
  Dog dog("beibei", 3, 5, 10);
  dog.eat();
  dog.run();

  return 0;
}
