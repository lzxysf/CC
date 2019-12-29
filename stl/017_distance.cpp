#include <iostream>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
  map<int,string> mymap;
  mymap.insert(make_pair(1, "one"));
  mymap.insert(make_pair(2, "two"));
  mymap.insert(make_pair(3, "three"));
  mymap.insert(make_pair(4, "four"));

  map<int,string>::iterator it = mymap.begin();
  for(it; it != mymap.end(); it++)
  {
    cout << it->first << it->second << endl;
  }

  //下面这句语法错误，map的迭代器不能直接相减
  // int ret = mymap.begin() - mymap.end();

  //distance可以计算两个迭代器之间的差值
  int count = distance(mymap.begin(), mymap.end());
  cout << "map的大小为" << count << endl;

  map<int,string>::iterator pos = mymap.find(3);
  int position = distance(mymap.begin(), pos);
  cout << "map中key为3的元素所在位置为" << position << endl;

  return 0;
}
