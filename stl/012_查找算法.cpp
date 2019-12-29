#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
InputIterator find (InputIterator beg, InputIterator end, const T& value);
InputIterator find_if (InputIterator beg, InputIterator end, UnaryPredicate op);
InputIterator find_if_not (InputIterator beg, InputIterator end, UnaryPredicate op);
find函数共有上面三种形式，这三种形式都是对于区间元素逐个比较，直到找到符合条件的元素，所以该函数是线性复杂度，第一种返回的是区间中第一个等于value元素的位置，第二个返回第一个令op返回true的位置，第三种形式返回的是第一个令op返回false的位置，如果没有找到符合规则的元素，这三种形式都将返回end。
*/

//InputIterator find (InputIterator beg, InputIterator end, const T& value);
//返回区间中第一个等于value的元素所在位置
void test1()
{
  vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);

  vector<int>::iterator it = find(v.begin(), v.end(), 5);
  if(it != v.end())
  {
    cout << "找到了" << endl;
  }
  else
  {
    cout << "没有找到" << endl;
  }
}

//若容器中的元素无法进行==比较，则无法使用find函数进行查找，因为find内部会对容器中元素进行==判断
//要想使用find需要对==进行重载
class Student
{
public:
  Student(int id, int age):id(id),age(age){}
  int id;
  int age;

  bool operator==(const Student& s)
  {
    if(s.id == this->id && s.age == this->age)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
};

void test2()
{
  Student s1(1,1),s2(2,2),s3(3,3);
  vector<Student> v_s;
  v_s.push_back(s1);
  v_s.push_back(s2);
  v_s.push_back(s3);
  vector<Student>::iterator it = find(v_s.begin(), v_s.end(), Student(1,1));
  if(it != v_s.end())
  {
    cout << "找到了" << endl;
    cout << (*it).id << "," << (*it).age << endl;
  }
  else
  {
    cout << "没有找到" << endl;
  }
}

//InputIterator find_if (InputIterator beg, InputIterator end, UnaryPredicate op)
//返回区间中第一个使op为true的元素
bool isOdd(int v)
{
  return v%2;
}

void test3()
{
  vector<int> v;
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(1);

  //返回第一个奇数所在位置
  vector<int>::iterator it = find_if(v.begin(), v.end(), isOdd);
  if(it != v.end())
  {
    cout << "找到了:" << *it << endl;
  }
  else
  {
    cout << "没有找到" << endl;
  }
}

int main()
{
  // test1();
  // test2();
  test3();
  return 0;
}
