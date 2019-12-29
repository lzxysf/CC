#include <iostream>
#include <string>

using namespace std;

//pair的初始化
void test1()
{
  //第一种方式
  pair<int, string> mypair1(1, "hello");
  cout << mypair1.first << " " << mypair1.second << endl;

  //第二种方式
  pair<int, string> mypair2 = make_pair(2, "world");
  // mypair2 = make_pair(2, "WORLD");
  cout << mypair2.first << " " << mypair2.second << endl;

  //第三种方式
  pair<int, string> mypair3 = mypair2;
  cout << mypair3.first << " " << mypair3.second << endl;
}


int main()
{
  test1();
  return 0;
}
