#include <iostream>
#include <stack>
#include <deque>
#include <vector>

using namespace std;

//stack-栈，是一种后进先出LIFO的数据结构，只能在栈顶进行插入和取出，需要stack头文件


//stack的初始化
void test1()
{
  stack<int> s;//构造一个用于存放int类型的空栈，默认底层容器是deque，size为0
  
  deque<int> mydeque(3, 100);//构造一个包含3个int元素的双端队列
  stack<int> s1(mydeque);//用自己的双端队列构造一个栈（size=3）

  stack<int, vector<int> > s2;//指明用vector容器实现一个栈(存放int)，空栈size=0
  //注意，两个>号之间要有空格

  vector<int> myvector(3,100);//构造一个包含3个int元素的vector
  stack<int, vector<int> > s3(myvector);//用自己的vector构造一个栈(size=3)
}

//push,pop
void test2()
{
  stack<string> s;
  s.push("func1");
  s.push("func2");
  s.push("func3");

  while(!s.empty())
  {
    cout << s.top() << endl;//返回栈顶元素，但是不删除
    s.pop();//pop将栈顶元素删除，没有返回值
  }
}

int main()
{
  // test1();
  test2();
  return 0;
}