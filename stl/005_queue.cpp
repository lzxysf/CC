#include <iostream>
#include <queue>
#include <string>

using namespace std;

//queue-队列，是一种先进先出FIFO的数据结构，在队列尾部插入元素，在队列头部取出元素

int main()
{
  queue<string> q1;
  q1.push("notify1");
  q1.push("notify2");
  q1.push("notify3");
  
  queue<string> q2(q1);//拷贝构造

  while(q2.size()>0)
  {
    cout << q2.front() << " ";
    q2.pop();
  }
  cout << endl;

  return 0;
}