#include <iostream>

using namespace std;

template <typename T>
const T& max(const T& a, const T& b)
{
	return a > b ? a : b;
}

int main()
{
	//gcc中默认是有max函数的，因此加上::，意思是调用当前空间的max函数
	//或者也可以将max改名为mymax
	int ret1 = ::max(3, 5);
	cout << ret1 << endl;
	
	//double ret2 = ::max(7.8, 111.2);//自动推导调用
	double ret2 = ::max<double>(7.8, 111.2);//显式调用
	cout << ret2 << endl;
	
	char ret3 = ::max('A', '8');
	cout << ret3 << endl;
	
	string ret4 = ::max("hello", "world");
	cout << ret4 << endl;
	
	return 0;
}
