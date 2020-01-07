#include <iostream>

using namespace std;

#if 0
//test是类的成员函数，因此是在类内部，可以直接调用类的私有成员即obj.a obj.b
class Obj
{
public:
    Obj(int a, int b) : a(a), b(b) {}
    void test(const Obj& obj)
    {
        cout << obj.a << " " << obj.b << endl;
    }
private:
    int a;
    int b;
};

int main()
{
    Obj obj(1,3);
    obj.test(obj);
    return 0;
}
#else
//此处test是友元函数，不是类的成员函数，不在类内部.但是由于是友元函数，因此也可以直接调用类的私有成员
class Obj
{
public:
    Obj(int a, int b) : a(a), b(b) {}
    friend void test(const Obj& obj)
    {
        cout << obj.a << " " << obj.b << endl;
    }
private:
    int a;
    int b;
};

int main()
{
    Obj obj(1,3);
    test(obj);
    return 0;
}

#endif
