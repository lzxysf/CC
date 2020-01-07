#include <iostream>

using namespace std;

/*
函数模板可以定义任意多个不同类型参数，但是对于多参数函数模板
-编译器是无法自动推导返回值类型的
-可以从左向右部分指定类型参数（指定的第一个即返回值类型）
*/

/*
总结
    - 函数模板是泛型编程在C++中的应用方式之一
    - 函数模板能够根据实参对参数类型进行推导
    - 函数模板支持显示的指定参数类型
    - 函数模板是C++中重要的代码复用方式
    - 函数模板通过具体类型产生不同的函数
    - 函数模板可以定义任意多个不同的类型参数
    - 函数模板中的返回值类型必须显示指定
    - 函数模板可以像普通函数一样重载
*/

template <typename T1, typename T2, typename T3>
T1 max(const T2& a, const T3& b)
{
	T1 m = a > b ? a : b;
	return m;
}

int main()
{
	int a = 12;
	char b = 'b';
	//cout << ::max(a, b) << endl; 			//error,无法自动推导出返回值
	cout << ::max<int>(a, b) << endl;		//返回值在第一个类型参数中指定
	cout << ::max<int, int, char>(a, b) << endl;
	return 0;
}
