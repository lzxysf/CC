#include <iostream>

/*
您可能会写一个名为 xyz() 的函数，在另一个可用的库中也存在一个相同的函数 xyz()。这样，编译器就无法判断您所使用的是哪一个 xyz() 函数。
因此，引入了命名空间这个概念，专门用于解决上面的问题，它可作为附加信息来区分不同库中相同名称的函数、类、变量等。使用了命名空间即定义了上下文。本质上，命名空间就是定义了一个范围。
*/

using namespace std;

namespace first_space{
	void func()
	{
		cout << "inside first space" << endl;
	}
}

namespace second_space{
	void func()
	{
		cout << "inside second space" << endl;
	}
}

void test1()
{
	first_space::func();
	second_space::func();
}

void test2()
{
	using namespace first_space;
	func();
}

//命名空间是可以嵌套的
namespace A
{
	int a = 100;
	void func()
	{
		cout << "a=" << a << endl;
	}
	
	namespace B
	{
		int a = 20;
		void func()
		{
			cout << "a=" << a << endl;
		}
	}
}

void test3()
{
	A::func();
	A::B::func();
}

void test4()
{
	using namespace A::B;
	func();
}

void test5()
{
	//using指令可以指定命名空间中的特定项目，例如只打算使用命名空间A中的func项目。
	//这样，调用func时前面就不需要命名空间A修饰，但是调用A中的其它项目仍然需要命名空间。
	using A::func;
	A::a = 250;
	func();
}

int main()
{
	//test1();
	//test2();
	//test3();
	//test4();
	test5();
	return 0;
}
