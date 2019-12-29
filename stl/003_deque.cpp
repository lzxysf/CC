#include <iostream>
#include <deque>

using namespace std;

//deque和vector非常相似，都是序列式容器，都是采用动态数组管理元素，提供随机存储，并且有着和vector一样的接口
//不同的是deque有着首尾两端快速删除的能力

//数据访问
void test1()
{
  deque<int> d;
  for(int i = 0; i < 10; i++)
  {
    d.push_back(i);
  }

  cout << d.at(5) << endl;
  cout << d.front() << endl;
  cout << d.back() << endl;

  deque<int>::iterator pstart = d.begin();
  deque<int>::iterator pstop = d.end();
  while(pstart != pstop) 
  {
    cout << *pstart;
    pstart++;
  }
  cout << endl;

  deque<int>::reverse_iterator rit_start = d.rbegin();
  deque<int>::reverse_iterator rit_end = d.rend();
  
  cout << *rit_start << endl;
  while(rit_start != rit_end)
  {
    cout << *rit_start;
    rit_start++;
  }
  cout << endl;
}

//加入数据
void test2()
{
  deque<int> d;
  d.push_back(1);
  d.push_back(2);
  d.push_front(3);
  d.push_front(4);
  d.insert(d.begin()+1, 99);

  for(int i = 0; i < d.size(); i++)
  {
    cout << d.at(i) << " ";
  }
  cout << endl;
}

//删除数据
void test3()
{
 int arr[] = {3,1,4,1,5};
 deque<int> d(arr, &arr[sizeof(arr)/sizeof(int)]);
 for(int i = 0; i < d.size(); i++)
 {
   cout << d[i] << " ";
 }
 cout << endl;
 
 //后端删除
 d.pop_back();
 for(int i = 0; i < d.size(); i++)
 {
   cout << d[i] << " ";
 }
 cout << endl;

//前端删除
 d.pop_front();
 for(int i = 0; i < d.size(); i++)
 {
   cout << d[i] << " ";
 }
 cout << endl;

//清空
 d.clear();
 cout << "是否为空:" << d.empty() << endl;
}

int main()
{
  // test1();
  // test2();
  test3();
  return 0;
}
