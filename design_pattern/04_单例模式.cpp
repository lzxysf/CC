#include <iostream>
#include <pthread.h>

using namespace std;

pthread_mutex_t mutext = PTHREAD_MUTEX_INITIALIZER;

//饿汉式,单例类一创建就实例化了
//饿了肯定要饥不择食,所以在单例类定义的时候就进行实例化
class Singleton
{
public:
  static Singleton* get_instance()
  {
    return instance;
  }
private:
  Singleton(){}
  static Singleton* instance;
};
Singleton* Singleton::instance = new Singleton();

//懒汉式，类在第一次调用时才初始化，节省系统资源，但是每次都需要判断是否创建
//懒汉：看名字就知道，不到万不得已就不会去实例化类，也就是说在第一次用到类实例的时候才会去实例化
class Singleton2
{
public:
  static Singleton2* get_instance()
  {
    if(instance == NULL)
    {
      instance = new Singleton2();
    }
    return instance;
  }
private:
  Singleton2(){}
  static Singleton2* instance;
};
Singleton2* Singleton2::instance = NULL;

//懒汉式的单例模式是线程不安全的，最简单的办法就是加锁
class Singleton3
{
public:
  static Singleton3* get_instance()
  {
    pthread_mutex_lock(&mutext);
    if(instance == NULL)
    {
      instance = new Singleton3();
    }
    pthread_mutex_unlock(&mutext);
    return instance;
  }
private:
  Singleton3(){}
  static Singleton3* instance;
};
Singleton3* Singleton3::instance = NULL;

//懒汉式的设计模式要想线程安全，还可以使用静态局部变量
//因为静态局部变量只会创建一次
class Singleton4
{
public:
  static Singleton4* get_instance()
  {
    static Singleton4* instance = new Singleton4();
    return instance;
  }
private:
  Singleton4(){}
};


int main()
{
  Singleton* s1 = Singleton::get_instance();
  Singleton* s2 = Singleton::get_instance();
  cout << s1 << endl << s2 << endl;

  Singleton2* s3 = Singleton2::get_instance();
  Singleton2* s4 = Singleton2::get_instance();
  cout << s3 << endl << s4 << endl;

  Singleton3* s5 = Singleton3::get_instance();
  Singleton3* s6 = Singleton3::get_instance();
  cout << s5 << endl << s6 << endl;

  Singleton4* s7 = Singleton4::get_instance();
  Singleton4* s8 = Singleton4::get_instance();
  cout << s7 << endl << s8 << endl;

  return 0;
}
