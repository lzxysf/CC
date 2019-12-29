#include <iostream>
#include <map>
#include <string>

using namespace std;

/*
map存储key-value数据，并根据key进行排序，map中key值不能重复
可以修改map的value值，不能修改key值
*/

//map容器初始化
void test1()
{
  map<int,string> mymap1;//默认构造
  map<int,string> mymap2(mymap1);//拷贝构造
}

//map插入操作
void test2()
{
  map<int, string> mymap;

  //第一种插入方法
  pair<map<int,string>::iterator,bool> ret = mymap.insert(pair<int,string>(1,"one"));
  //ret的第一个参数是插入位置，第二个参数是是否插入成功
  cout << "插入的数据是" << ret.first->first << ":" << ret.first->second << "插入是否成功:" << ret.second << endl;

  //第二种插入方法
  mymap.insert(make_pair(2, "two"));

  //第三种插入方法
  mymap.insert(map<int,string>::value_type(3, "three"));

  //第四种插入方式
  mymap[4] = "four";

  for(map<int,string>::iterator it = mymap.begin(); it != mymap.end(); it++)
  {
    cout << it->first << ":" << it->second << endl;
  }

  //第四种插入方式，即数组插入方式和其它插入方式有区别
  //其它方式，如果插入一个相同key值的元素，这个插入无效，会被抛弃
  //数组插入方式，如果插入一个相同key值的元素，会覆盖已经存在的元素

  mymap.insert(pair<int,string>(1,"ONE"));//插入无效
  mymap[3] = "THREE";//覆盖了旧的元素
  for(map<int,string>::iterator it = mymap.begin(); it != mymap.end(); it++)
  {
    cout << it->first << ":" << it->second << endl;
  }

}

//map删除操作，按照key值删除
void test3()
{
  map<int,string> mymap;
  mymap.insert(make_pair(1,"hello"));
  mymap.insert(make_pair(2,"world"));
  mymap.insert(make_pair(3,"kitty"));
  mymap.insert(make_pair(4,"ming"));

  mymap.erase(1);//删除key为1的元素
  mymap.erase(mymap.begin());//删除第一个元素
  // mymap.erase(mymap.begin(),mymap.end());//mymap.clear()
  // mymap.clear();

  for(map<int,string>::iterator it = mymap.begin(); it != mymap.end(); it++)
  {
    cout << it->first << ":" << it->second << endl;
  }
}

//map查找，按照key值查找
void test4()
{
  map<int,string> mymap;
  mymap.insert(make_pair(1,"hello"));
  mymap.insert(make_pair(2,"world"));
  mymap.insert(make_pair(3,"kitty"));
  mymap.insert(make_pair(4,"ming"));

  map<int,string>::iterator it = mymap.find(100);
  if(it == mymap.end())
  {
    cout << "没有找打" << endl;
  }
  else
  {
    cout << "找到了" << endl;
  }
}

int main()
{
  // test1();
  // test2();
  // test3();
  test4();
  return 0;
}
