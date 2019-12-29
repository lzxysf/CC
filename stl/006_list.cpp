#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

//list-双向链表，在内存上非连续存储。有迭代器。可方便的在任意位置插入和删除元素，但空间和时间开销较大
//list的使用方法和vector等是类似的，但是它没有[]和at方法，就是说list不能随机访问（list是链式结构，随机访问效率不高，链式结构的优势在于在任意的位置插入和删除元素）

class Print
{
public:
  void operator()(int v)
  {
    cout << v << " ";
  }
};

void print(int v)
{
  cout << v << " ";
}

int main()
{
  list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_front(3);
  l.push_front(4);

  for(list<int>::iterator it = l.begin(); it != l.end(); it++)
  {
    cout << *it << " ";
  }
  cout << endl;

  l.insert(l.begin(),100);
  l.insert(l.begin()++,2,500);//此处为++，不能用+=或者直接+某个数

  for(list<int>::iterator it = l.begin(); it != l.end(); it++)
  {
    cout << *it << " ";
  }
  cout << endl;

  cout << l.front() << endl;
  l.pop_front();
  cout << l.back() << endl;
  l.pop_back();
  for_each(l.begin(), l.end(), Print());//for_each需要引入algorithm头文件
  cout << endl;

  l.sort();//排序，同样需要algorithm头文件
  for_each(l.begin(), l.end(), print);
  cout << endl;

  return 0;
}
