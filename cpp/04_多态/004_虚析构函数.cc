#include <iostream>

using namespace std;

/*
虚析构函数是为了避免内存泄漏，而且是当子类中会有指针成员变量时才会使用得到。
也就是说虚析构函数使得在删除指向子类对象的基类指针时可以调用子类的析构函数达到释放子类中堆内存的目的，以防止内存泄漏。
*/

/*
如果析构函数不加virtual, 那么调用顺序是：
父类构造函数
子类构造函数
父类析构函数
如果加virtual，那么调用顺序是：
父类构造函数
子类构造函数
子类析构函数
父类析构函数
*/

class A
{
public:
	A(int a)
	{
		cout << "父类构造函数" << endl;
		m_a = a;
	}
	virtual ~A()
	{
		cout << "父类析构函数" << endl;
	}
private:
	int m_a;
};

class B : public A
{
public:
	B(int a, int b) : A(a)
	{
		cout << "子类构造函数" << endl;
		p = new int;
		*p = b;
	}
	~B()
	{
		cout << "子类析构函数" << endl;
		if(p)
		{
			delete p;
			p = NULL;
		}
	}
private:
	int* p;
};

int main()
{
	
	A* a = new B(1, 2);
	delete a;
	
	return 0;
}
