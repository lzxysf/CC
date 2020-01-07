#include <iostream>
#include <stdio.h>

using namespace std;

class Obj
{
public:
    Obj(int num)
    {
        ptr = new int(num);
    }
    Obj(const Obj& obj)
    {
        cout << "拷贝构造"  << endl;
        ptr = new int(*obj.ptr);
    }

    int *ptr;
};

void test(Obj obj)
{
    cout << *obj.ptr << endl;
}

Obj test2(Obj obj)
{
    return obj;
}

int main()
{
    Obj obj(10);
    
    //场景1
    Obj obj2 = obj;//拷贝构造
    printf("%p\r\n", obj.ptr);
    printf("%p\r\n", obj2.ptr);

    //场景2
    Obj obj3(obj2);//拷贝构造

    //场景3，类对象作为函数参数，但如果是类指针或类引用就不会进行拷贝构造
    test(obj3);//拷贝构造

    //场景4，类对象作为返回值
    Obj obj4 = test2(obj3);//拷贝构造,拷贝构造
}
