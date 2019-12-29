#include <iostream>
#include <set>

using namespace std;

//set也是一个存储相同数据类型的容器，set的特点有:存储的数据是唯一的，存储的数据进行自动排序
//由于set是有序的，因此set不能存放无法比较大小的元素
//stl中的set/multiset/map/multimap内部都采用了一种非常高效的平衡二叉树-红黑树(RB树)
//set可以用于插入、删除和检索，而且是十分高效的。因为set使用了二叉树，所以这些操作都是基于二分法的，也就是说即使有10000个数据需要检索，那么也只是需要14次比较运算即可，2^14=16384
//set中元素的值不能直接被改变，因为set是有序的，直接改变其值会打乱其结构
//iterator相当于指向节点的指针，set的内存没有变，指向内存的指针也不会失效(当然被删除的那个元素本身已经失效了)。
//但是对于vector来说，每一次删除和插入，指针都有可能失效，调用push_back在尾部插入也是如此。因为为了保证内部数据的连续存放，iterator指向的那块内存在删除和插入过程中可能已经被其他内存覆盖或者内存已经被释放了。push_back的时候，容器内部空间可能不够，需要一块新的更大的内存，只有把以前的内存释放，申请新的更大的内存，复制已有的数据元素到新的内存，最后把需要插入的元素放到最后，那么以前的内存指针自然就不可用了。牢记这个原则：不要使用过期的iterator。

//insert,size,max_size,clear,empty
void test1()
{
  set<int> myset;
  myset.insert(1);
  myset.insert(3);
  myset.insert(3);
  myset.insert(2);
  cout << "set的大小:" << myset.size() << endl;//3
  cout << "set的max_size:" << myset.max_size() << endl;//461168601842738790
  cout << "set中的第一个元素:" << *myset.begin() << endl;
  cout << "set中的最后一个元素:" << *myset.end() << endl;
  
  myset.clear();
  cout << "set是否为空：" << myset.empty() << endl;
  cout << "set的大小:" << myset.size() << endl;
  cout << "set的max_size:" << myset.max_size() << endl;
 }

//count()用来查找某个键值出现的次数，这个函数在set中并不实用，因为set中键值只可能出现0或1次，这样就成为了判断该键值是否在set中存在了
void test2()
{
  set<int> myset;
  myset.insert(1);
  myset.insert(3);
  myset.insert(3);
  myset.insert(2);

  cout << "set中是否存在2:" << myset.count(2) << endl;
 }


class Student
{
  public:
    Student(int id, int age, string name):id(id),age(age),name(name) {}
    friend bool operator < (const Student s1, const Student& s2)
    {
      return s1.id < s2.id;
    }
    void print()
    {
      cout << "id:" << id << " age:" << age << " name:" << name << endl;
    }
  private:
    int id;
    int age;
    string name;
};

//set无法存储不能比较大小的数据，set内部通过小于号<比较元素大小，因此如果想存储无法比较大小的元素，需要重载运算符<,重载运算符的形式如上
void test3()
{
  set<Student> myset;
  Student s1(1,12,"liming"), s2(2,13,"wangming"), s3(3,15,"liuli");
  myset.insert(s1);
  myset.insert(s3);
  myset.insert(s2);

  for(set<Student>::iterator it = myset.begin(); it != myset.end(); it++)
  {
    Student myset = Student(*it);
    myset.print();
  }
}

void test4()
{
  set<int> myset;
  myset.insert(1);
  myset.insert(3);
  myset.insert(3);
  myset.insert(2);

  //set.find返回迭代器，指向set.end()说明set中没有这个值，否则说明set中有这个值
  set<int>::iterator pos = myset.find(100);
  if(pos == myset.end())
  {
    cout << "没有找到" << endl;
  }
  else
  {
    cout << "找到了" << endl;
  }
}

void test5()
{
  set<int> myset;
  myset.insert(1);
  myset.insert(3);
  myset.insert(3);
  myset.insert(2);
  myset.insert(5);
  myset.insert(10);
  myset.insert(20);

  //lower_bound(n)返回指向n的第一个迭代器,未找到则返回set.end()
  //由于set中不能存放相同元素，因此lower_bound就是指向set中唯一的n的迭代器
  set<int>::iterator it = myset.lower_bound(5);
  for(it; it != myset.end(); it++)
  {
    //cout << *it << endl;
  }

  //upper_bound返回指向5的最后一个迭代器的下一个位置，未找到则返回set.end()
  it = myset.upper_bound(5);
  for(set<int>::iterator pstart = myset.begin(); pstart != it; pstart++)
  {
    // cout << *pstart << endl;
  }

  //equal_range(n)返回pair<第一个n位置的迭代器，最后一个n的下一个位置的迭代器>
  pair<set<int>::iterator, set<int>::iterator> mypair;
  mypair = myset.equal_range(5);
  for(set<int>::iterator it = mypair.first; it != mypair.second; it++)
  {
    cout << *it << endl;
  }
}

int main()
{
  // test1();
  // test2();
  // test3();
  // test4();
  test5();
  return 0;
}
