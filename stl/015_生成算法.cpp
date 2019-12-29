#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

void print(int v)
{
    cout << v << " ";
}

//accumulate累加，需要导入numeric头文件
void test1()
{
  vector<int> v;
  for(int i = 0; i < 100; i++)
  {
    v.push_back(i);
  }
  //累加，前两个参数标识累加元素的范围，第三个元素标识累加的初始值
  int sum = accumulate(v.begin(), v.end(), 0);
  cout << sum << endl;
}

//fill,填充
void test2()
{
  vector<int> v;
  v.resize(10);
  fill(v.begin(), v.end(), 100);//v中的每个元素都填充为100
  for_each(v.begin(), v.end(), print);
}

int main()
{
//   test1();
  test2();
  return 0;
}
