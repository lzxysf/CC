#include <iostream>
#include <string.h>

using namespace std;

/*
当类中有指针变量（手动开辟堆内存空间）时，需要实现拷贝构造函数和赋值运算符重载。
注意：49行和53行，一个是拷贝构造，一个是赋值运算，注意区分。
*/

class Student
{
public:
	Student(int id, char* name)
	{
		this->id = id;
		this->name = strdup(name);
	}
	Student(const Student& student)
	{
		cout << "拷贝构造函数" << endl;
		this->id = student.id;
		this->name = strdup(student.name);
	}
	Student& operator=(const Student& student)
	{
		cout << "赋值运算符重载" << endl;
		this->id = student.id;
		this->name = strdup(student.name);
		return *this;
	}
	void print()
	{
		cout << "student, id = " << id << ",name= " << name << endl;
	}
	int id;
	char* name;
};

int main()
{
	char* name = strdup("liming");
	Student s1(1, name);
	printf("s1.name地址:%p\r\n", s1.name);
	
	Student s2(s1);	//拷贝构造
	printf("s2.name地址:%p\r\n", s2.name);
	
	Student s3 = s2; //拷贝构造
	printf("s3.name地址:%p\r\n", s3.name);
	
	Student s4(4, "");
	s4 = s3;	//=赋值运算，此处需要重载赋值运算符
	printf("s4.name地址:%p\r\n", s4.name);
	
	return 0;
}
