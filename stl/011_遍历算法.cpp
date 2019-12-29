#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print1(int v)
{
    cout << v << " ";
}

class print2
{
public:
  void operator()(int v)
  {
    cout << v << " ";
  }
};

//for_each用于遍历，需要导入algorithm头文件
void test1()
{
  vector<int> v;
  for(int i = 0;i < 10; i++)
  {
      v.push_back(i);
  }
  for_each(v.begin(), v.end(), print1);
  cout << endl;
  for_each(v.begin(), v.end(), print2());
  cout << endl;
}

//transform函数的作用是：将某操作应用于指定范围的每个元素
//eg.利用transform函数将一个给定的字符串中的小写字母改写成大写字母，并将结果保存在一个叫second的数组里，原字符串内容不变。
class Upper
{
public:
  char operator()(char c)
  {
    if(c>='A' && c<='Z')
    {
      return c + 32;
    }
    else
    {
      return c;
    }
  }
};
void test2()
{
  string first,second;
  cin >> first;
  second.resize(first.size());//必须给second分配内存空间
  transform(first.begin(),first.end(),second.begin(), Upper());
  cout << second << endl;
}


//eg.利用tramsform将两个vector每个元素相乘
class Multiply
{
public:
  int operator()(int v1, int v2)
  {
    return v1*v2;
  }
};
void test3()
{
  vector<int> v1, v2, result;
  for(int i = 0; i < 10; i++)
  {
    v1.push_back(i);
  }
  v2 = v1;
  
  //同样，必须给result分配内存空间
  result.resize(v1.size());
  transform(v1.begin(),v1.end(),v2.begin(),result.begin(),Multiply());
  for_each(result.begin(),result.end(),print1);
}

int main()
{
  // test1();
  // test2();
  test3();
  return 0;
}
