#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print(int v)
{
  cout << v << " ";
}

void test1()
{
  vector<int> v1,v2,v3;
	v1.push_back(6);
	v1.push_back(2);
	v1.push_back(8);
	v1.push_back(4);

	v2.push_back(1);
	v2.push_back(2);
	v2.push_back(6);
	v2.push_back(4);

  //sort less greater merge
  // sort(v1.begin(), v1.end(), less<int>());//less表示升序，此参数可省略
  sort(v1.begin(), v1.end());//默认升序排列
  for_each(v1.begin(), v1.end(), print);
  cout << endl;

  sort(v2.begin(), v2.end(), greater<int>());//greater表示大的在前，即降序排列
  for_each(v2.begin(), v2.end(), print);
  cout << endl;

  //空的vector在使用前必须使用resize开辟空间
  v3.resize(v1.size()+v2.size());
  for_each(v3.begin(), v3.end(), print);
  cout << endl;
  //merge用于合并两个sorted range 的元素 [first1,end1) [first2, end2)
  //这两个range需要为相同的排序规则，即都是降序或都是升序
  //merge后的v3元素中按顺序存放两个range中所有元素
  merge(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
  for_each(v3.begin(), v3.end(), print);
  cout << endl;
}

//random_shuffle,洗牌，将容器中的元素顺序打乱
void test2()
{
  vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);
  for_each(v1.begin(), v1.end(), print);
  cout << endl;

  random_shuffle(v1.begin(), v1.end());
  for_each(v1.begin(), v1.end(), print);
  cout << endl;
  random_shuffle(v1.begin(), v1.end());
  for_each(v1.begin(), v1.end(), print);
  cout << endl;
}

//reverse,反转容器中的元素
void test3()
{
  vector<int> v;
  for(int i = 0; i < 10; i++)
  {
    v.push_back(i);
  }

  for_each(v.begin(), v.end(), print);
  cout << endl;

  reverse(v.begin(), v.end());
  for_each(v.begin(), v.end(), print);
  cout << endl;
}

int main()
{
  // test1();
  // test2();
  test3();
  return 0;
}
