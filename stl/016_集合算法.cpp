#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print(int v)
{
  cout << v << " ";
}

//set_intersection 求两个集合的交集
//求交集的两个集合必须是sorted的，即有序的
void test1()
{
  vector<int> v1, v2, v3;
  v1.push_back(2);
  v1.push_back(5);
  v1.push_back(6);
  v1.push_back(8);
  v1.push_back(10);
  v1.push_back(11);
  v1.push_back(12);

  v2.push_back(6);
  v2.push_back(11);
  v2.push_back(12);
  v2.push_back(13);

  v3.resize(min(v1.size(), v2.size()));//v3的大小必定不大于最小的那个集合，但若要求得v3精确的大小可见test2
  set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());

  for_each(v3.begin(), v3.end(), print);
}

//计算所求交集v3的实际大小
void test1_1()
{
  vector<int> v1, v2, v3;
  v1.push_back(2);
  v1.push_back(5);
  v1.push_back(6);
  v1.push_back(8);
  v1.push_back(10);
  v1.push_back(11);
  v1.push_back(12);

  v2.push_back(6);
  v2.push_back(11);
  v2.push_back(12);
  v2.push_back(13);

  v3.resize(min(v1.size(), v2.size()));
  vector<int>::iterator it = set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
  v3.resize(it - v3.begin());

  for_each(v3.begin(), v3.end(), print);
}

//set_union 求两个集合的并集，两个集合必须是有序的
void test2()
{
  vector<int> v1, v2, v3;
  v1.push_back(2);
  v1.push_back(5);
  v1.push_back(6);
  v1.push_back(8);
  v1.push_back(10);
  v1.push_back(11);
  v1.push_back(12);

  v2.push_back(6);
  v2.push_back(11);
  v2.push_back(12);
  v2.push_back(13);

  v3.resize(v1.size()+v2.size());
  set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
  
  for_each(v3.begin(), v3.end(), print);
  cout << endl;
}

//set_difference 求两个集合的差集，两个集合必须是有序的
void test3()
{
  vector<int> v1, v2, v3;
  v1.push_back(2);
  v1.push_back(5);
  v1.push_back(6);
  v1.push_back(8);
  v1.push_back(10);
  v1.push_back(11);
  v1.push_back(12);

  v2.push_back(6);
  v2.push_back(11);
  v2.push_back(12);
  v2.push_back(13);

  v3.resize(max(v1.size(),v2.size()));
  set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
  
  for_each(v3.begin(), v3.end(), print);
  cout << endl;
}

int main()
{
  // test1();
  // test1_1();
  // test2();
  test3();
  return 0;
}
