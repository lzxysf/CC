#include <iostream>
#include <set>

using namespace std;

//multiset与set的区别是：可以存放相同的元素

void test1()
{
  multiset<int> mset;
  mset.insert(1);
  mset.insert(5);
  mset.insert(5);
  mset.insert(10);

  multiset<int>::iterator it = mset.begin();
  for(it; it != mset.end(); it++)
  {
    cout << *it << endl;
  }
}

int main()
{
  test1();
  return 0;
}
