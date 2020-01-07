#include <iostream>

using namespace std;

/*
子类内部
    调用子类本身同名成员  “this->成员” 或 “成员”
    调用父类同名成员      “父类::成员”
子类外部
    调用子类本身同名成员  “子类对象.成员”
    调用父类同名成员      “子类对象.父类::成员”
*/

class Father
{
public:
    int m_a;
    void test()
    {
        cout << "father test" << endl;
    }
};

class Son : public Father
{
public:
    int m_a;
    void test()
    {
        cout << "son test" << endl;
    }
    void print()
    {
        m_a = 10;
        Father::m_a = 100;
        test(); //默认调用子类本身的函数，也可以写作this->test();
        Father::test(); //调用父类的同名函数
    }
};

int main()
{
    Son son;
    son.print();

    son.test();
    son.Father::test();

    son.m_a = 10;
    son.Father::m_a = 100;
    
    return 0;
}
