#include <iostream>

using namespace std;

/*C++多态性必须使用虚函数virtual来实现，否则父类的指针指向子类后向上转型了，调用的都是父类的方法，无法调用子类特有的方法*/

class Animal
{
  public:
    virtual void eat() {
      cout << "动物在吃" << endl;
    }
    virtual void sleep() {
      cout << "睡觉" << endl;
    }
  private:
};

class Dog : public Animal
{
  public:
    virtual void eat() {
      cout << "狗在吃骨头" << endl;
    }
};

class Cow : public Animal
{
  public:
    virtual void eat() {
      cout << "牛在吃草" << endl;
    }
};

int main()
{
  Animal *base = NULL;
  base = new Dog();
  base->eat();      //狗在吃骨头

  base = new Cow();
  base->eat();      //牛在吃草

  /*如果一个类中定义虚函数，C++编译器会在这个类中创建vptr指针，指向一个vtable函数表，此时如果使用sizeof求这个类所占空间，会发现多了一个指针的长度*/
  /*在每一个声明了虚函数或继承了虚函数的类中都有一个vptr指针指向一个vtalbe函数表*/
  /*如果一个类A继承了多个包含虚函数的类，类A中就包含多个vptr，同时指向多个vtable*/
 /*vptr构造的时间都是在，所有基类的构造函数完成后，自身构造函数开始前*/
  cout << sizeof(Animal) << endl;
  cout << sizeof(void*) << endl;
  return 0;
}
