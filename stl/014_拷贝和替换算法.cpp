#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

void print(int v)
{
    cout << v << " ";
}

//copy
void test1()
{
//   int* arr = new int[5]{1, 2, 3, 4, 5};
//或者
  int arr[5] = {0, 2, 3, 4, 5};
  
  vector<int> v;
  v.resize(5);
  copy(arr, arr+5, v.begin());
  for_each(v.begin(), v.end(), print);
}

//replace
void test2()
{
  string str = "liming is a good boy";
  char* str1 = "abcdefg";
//   str.replace(str.begin(), str.begin()+6, str1, 6);//同下
  str.replace(0, 6, str1, 6);//用str1的前6个字符替换str的0～6个字符

  char str2 = '#';
  str.replace(0, 6, 6, str2);//用6个连续的str2字符替换str的0～6个字符
  
  cout << str << endl;
}

//swap
void test3()
{
    vector<int> v1,v2;
    for(int i = 0; i < 10; i++)
    {
        v1.push_back(i);
    }
    for(int i = 20; i < 30; i++)
    {
        v2.push_back(i);
    }

    for_each(v1.begin(), v1.end(), print);
    cout << endl;
    for_each(v2.begin(), v2.end(), print);
    cout << endl;

    swap(v1, v2);
    for_each(v1.begin(), v1.end(), print);
    cout << endl;
    for_each(v2.begin(), v2.end(), print);
    cout << endl;
}

int main()
{
  // test1();
//   test2();
  test3();
  return 0;
}
