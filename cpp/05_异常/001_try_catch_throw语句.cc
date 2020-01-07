#include <iostream>

using namespace std;

/*
throw可以抛出基本类型的异常（还可以抛出自定义异常），然后由catch捕获
catch(...)捕获所有类型的异常
*/

class Obj
{
public:
	Obj(int a)
	{
		m_a = a;
	}
	friend void printObj(Obj* obj, int a, double b);
private:
	int m_a;
};

void printObj(Obj* obj, int a, double b)
{
	if(obj == NULL)
	{
		throw "NULLPointerException";
	}
	else if(a == 0)
	{
		throw 0;
	}
	else if(b == 0.0)
	{
		throw 0.0;
	}
	cout << "m_a=" << obj->m_a << endl;
}

int main()
{
	
	Obj* obj = NULL;
	obj = new Obj(10);
	try
	{
		printObj(obj, 0, 1);
	}
	catch(const char* msg)
	{
		cout << msg << endl;
	}
	/*
	catch(int a)
	{
		cout << a << endl;
	}
	catch(double a)
	{
		cout << a << endl;
	}*/
	catch(...)
	{
		cout << "捕获到异常" << endl;
	}
	return 0;
}
