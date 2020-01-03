#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

/*
port存放在类对象obj地址的开头
ip的首地址存放在类对象obj地址开头偏移8位
*/

class Object
{
private:
  int port;       //4
  char* ip;       //8
public:
  int get_port()
  {
    return port;
  }
  char* get_ip()
  {
    return this->ip;
  }
};

int main()
{
  Object obj;
  int* port_p = (int*)&obj;
  char** ip_p = (char**)((char*)&obj + 8);
  printf("%p\r\n", port_p);
  printf("%p\r\n", ip_p);

  *port_p = 5060;
  cout << "端口号为" << obj.get_port() << endl;

  *ip_p = "10.88.150.17";
  cout << "ip为" << obj.get_ip() << endl;

  return 0;
}
