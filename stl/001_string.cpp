#include <iostream>

using namespace std;

//char*和string相互转换
void test1()
{
  //string转换为const char*
  string s = "abc";
  const char* str = s.c_str();
  cout << str << endl;

  //char*转换为string
  char* str1 = "hello";
  string s1(str1);
  cout << s1 << endl;
}

//string初始化
void test2() 
{
  string s;
  string s2 = "abcd";
  string s3(s2);
  string s4(10, 'c');//10个字符‘c’组成的字符串

  cout << s2 << endl;
  cout << s3 << endl;
  cout << s4 << endl;
}

//string容器字符串赋值和存取
void test3()
{
  string s;
  s = "abcd";

  string s2;
  s2.assign("abcdef");

  string s3 = "abcdefg";
  for(int i = 0; i < s3.size(); i++) {
    cout << s3[i];
  }
  cout << endl;

  for(int i = 0; i < s3.size(); i++) {
    cout << s3.at(i);
  }
  cout << endl;

  //[]方式访问数组，访问越界会直接挂掉，不会抛异常
  //at方式访问数组，访问越界不会挂掉，会抛异常
  try{
    // cout << s3[100] << endl;
    cout << s3.at(100) << endl;
  }catch(...){
    cout << "访问越界" << endl;
  }

  cout << "test" << endl;
}

//string容器拼接
void test4() {
  string s1 = "aaa";
  string s2 = "bbb";

  string s3 = s1+s2;
  cout << s3 << endl;

  s1+=s2;
  cout << s1 << endl;

  //成员函数方法
  s1.append(s2);
  cout << s1 << endl;
}

//string查找和替换
void test5() {
  string s = "abcabcabc";
  string target = "bc";
  //从左向右查找，查找到的第一个匹配字符串的首地址
  int pos = s.find(target);
  cout << "pos:" << pos << endl;

  //从右向左查找，查找到的第一个匹配字符串的首地址
  int pos2 = s.rfind("bc");
  cout << "pos2:" << pos2 << endl;

  string s1 = "xy";
  s.replace(3,2,s1);//将位置3开始长度为2的字符串替换为s1
  cout << "s=" << s << endl;
}

//string 比较 子串 插入和删除
void test6() {
  string s1 = "abc";
  string s2 = "def";
  //s1>s2,返回值为正数;s1<s2,返回值为负数;s1==s2,返回值为0
  int ret = s1.compare(s2);
  cout << ret << endl;

  string s3 = s1.substr(1,2);
  cout << s3 << endl;

  s1.insert(0,s2);
  cout << s1 << endl;

  s1.erase(0,4);
  cout << s1 << endl;
}

int main()
{
  // test1();
  //test2();
  // test3();
  // test4();
  // test5();
  test6();
  return 0;
}