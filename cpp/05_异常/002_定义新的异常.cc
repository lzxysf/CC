#include <iostream>

using namespace std;

/*
exception 是所有标准C++异常的父类，可以通过继承和重载exception类来定义新的异常。
what()是异常类提供的一个公共方法，它已经被所有子异常重载。这将返回异常产生的原因。

const char * what () const throw ()
后边的const表示该成员函数不能修改成员变量
throw()是异常规格说明符，括号内写该函数可以抛出的异常类型，throw()括号内是空的，表示该函数不能返回任何异常。
函数后不加throw表示可以返回任意的异常。
*/

struct MyException : public exception
{
public:
	const char* what() const throw()
	{
		return "C++ Exception";
	}
};

int main()
{
	try
	{
		throw MyException();
	}
	catch(MyException& e)
	{
		cout << e.what() << endl;
	}
	catch(exception& e)
	{
		cout << e.what() << endl;
	}
	return 0;
}
