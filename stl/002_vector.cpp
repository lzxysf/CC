#include <iostream>
#include <vector>

using namespace std;

class Teacher{
  public:
  Teacher(int id, int age) : id(id), age(age){}
  int id;
  int age;
};

template <typename T>
void printVector(vector<T> v) {
  int size = v.size();
  for (int i = 0; i < size; i++) {
    cout << v.at(i) << " ";
  }
  cout << endl;
}

//vector demo
void test1() {
  vector<Teacher> v;//创建一个容器，指定里面放Teacher类型的数据

  Teacher t1(1, 21);
  Teacher t2(2, 25);
  Teacher t3(3, 28);

  v.push_back(t1);//在末尾处插入数据
  v.push_back(t2);
  v.push_back(t3);

  vector<Teacher>::iterator pstart = v.begin();//获得vector容器的开头的地址
  vector<Teacher>::iterator pstop = v.end();//获得vector容器的末尾的地址加1

  while(pstart != pstop)
  {
    Teacher t = *pstart;
    cout << "id:" << t.id << ",age:" << t.age << endl;
    pstart++;
  }

  cout << "hello world" << endl;
}

//vector初始化
void test2() {
  vector<int> v1;//创建一个空的vector容器
  vector<int> v2(10);//创建一个vector，元素个数为10
  vector<int> v3(10,2);//创建一个vector，元素个数为10,且值均为2
  vector<int> v4(v3.begin(), v3.end());//创建一个vector,复制v3种[begin,end)的元素到v4
  vector<int> v5(v4);//拷贝构造

  for(int i = 0;i < v2.size(); i++) {
    //此处也可以用v2[i]。使用at的好处是，如果访问越界不会直接崩溃，会抛出异常
    cout << v2.at(i);
  }
  cout << endl;

  for(int i = 0;i < v3.size(); i++) {
    cout << v3.at(i);
  }
  cout << endl;

  for(int i = 0;i < v4.size(); i++) {
    cout << v4.at(i);
  }
  cout << endl;

  for(int i = 0;i < v5.size(); i++) {
    cout << v5.at(i);
  }
  cout << endl;
}

void test3() {
  int arr[] = {3, 1, 4, 1, 5};
  vector<int> v(arr, &arr[sizeof(arr)/sizeof(int)]);
  for(int i = 0; i < v.size(); i++){
    cout << v.at(i);
  }
  cout << endl;
}

//vector赋值操作
void test4() {
  vector<int> v;
  v.assign(5, 1);

  vector<int> v1;
  v1.assign(v.begin(), v.end());

  vector<int> v2;
  v2 = v1;
}

//vector大小操作
//v.size()指容器当前所拥有的元素个数
//v.capacity()指容器在必须分配新的内存空间之前所能存放的元素总数
//v.resize(n,val)为指定大小，如果n小于vector的size则截断vector,如果n大于vector的size,vector中现有元素不变，新分配的元素值都为val（无val则调用默认构造，如int一般为0）,通过v.resize(n)设置v后v.size()必然为n
//v.reverse(n)为指定capacity大小,是预留空间，但不会真正创建元素
void test5() {
  vector<int> v;
  for (int i = 0; i < 10; i++) {
    v.push_back(i);
  }
  cout << "vector的size为" << v.size() << endl;
  cout << "vector的capacity为" << v.capacity() << endl;

  printVector(v);

  v.resize(5);
  printVector(v);
  cout << "resize(5)后v的size为" << v.size() << endl;
  cout << "resize(5)后v的capcity为" << v.capacity() << endl;

  v.resize(20);
  printVector(v);
  cout << "resize(20,100)后v的size为" << v.size() << endl;
  cout << "resize(20,100)后v的capcity为" << v.capacity() << endl;

  v.reserve(100);
  printVector(v);
  cout << "reserve(100)后v的size为" << v.size() << endl;
  cout << "reserve(100)后v的capcity为" << v.capacity() << endl;
}

//vector的插入和删除
void test7() {
  vector<int> v;
  for (int i = 0; i < 10; i++) {
    v.push_back(i);
  }
  printVector(v);
  
  //头部插入元素
  v.insert(v.begin(),100);
  printVector(v);

  //删除末尾的元素
  v.pop_back();
  printVector(v);

  //删除指定位置元素
  v.erase(v.begin());
  printVector(v);

  //区间删除
  v.erase(v.begin(), v.end()-1);
  printVector(v);
}

int main() {
  // test1();
  // test2();
  // test3();
  test5();
  //test7();
  return 0;
}
