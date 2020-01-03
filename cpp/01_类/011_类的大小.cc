#include <iostream>

using namespace std;

/*
类的大小与普通数据成员有关，与静态数据成员和成员函数都没有关系。
静态数据成员之所以不计算在类的对象大小内，是因为静态数据成员被该类所有的对象共享，不属于具体的某个对象
成员函数之所以不计算在类的对象大小之内，可由010文件看出，类对应的结构体是作为参数传入了成员函数中。
*/

struct Obj{
  int id;
  char* local_ip;
  char* server_ip;
  int port;
};

class Object1
{
  int id;
  char* local_ip;
  char* server_ip;
  int port;
};

class Object2
{
  int id;
  char* local_ip;
  char* server_ip;
  int port;
  static double abc;
  void func()
  {
    cout << "test" << endl;
  }
};

int main()
{
  cout << "结构体的大小" << sizeof(Obj) << endl;  //32
  cout << "类1的大小" << sizeof(Object1) << endl; //32
  cout << "类2的大小" << sizeof(Object2) << endl; //32

  return 0;
}
