#include <iostream>
#include <map>
#include <string.h>
#include <stdio.h>

using namespace std;

struct _callinfo{
  char udn[50];
  int cid;
};
typedef _callinfo callinfo;

ostream& operator<<(ostream& out, const pair<string,callinfo>& p)
{
  cout << p.first << ":" << p.second.udn << "," << p.second.cid << endl;
}

int main()
{
  callinfo info1 = {0};
  strcpy(info1.udn, "1001");
  info1.cid = 1;
  callinfo info2 = {0};
  strcpy(info2.udn, "1002");
  info2.cid = 2;

  map<string, callinfo> m;
  m.insert(pair<string,callinfo>("a", info2)); //map是按照key进行排序的，因此key必须是支持<运算符操作的
  m.insert(pair<string,callinfo>("b", info1));
  
  for(map<string,callinfo>::iterator it = m.begin(); it != m.end(); it++)
  {
    /*cout<<只能输出有限的数据类型,不是什么都能输出的,如果要输出自定义的结构体或类对象等，需要重载<<运算符，否则会报错，重载运算符见本文将开头*/
    cout << *it;
    // cout << it->first << endl;
    // cout << it->second.udn << endl;
    //*it.first----it->first
  }

  return 0;
}
