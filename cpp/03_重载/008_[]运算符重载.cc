#include <iostream>

/*
array[i]返回的是引用，可以作为左值，因此重载函数的返回值为int&
*/

using namespace std;

class Array
{
public:
    Array()
    {
        for(int i = 0; i < 10; i++)
        {
            array[i] = i * 2;
        }
    }
    int& operator[](int i)
    {
        if(i >= 10 || i < 0)
        {
            cout << "超出索引值" << endl;
            return array[0];
        }
        return array[i];
    }
private:
    int array[10];
};

int main()
{
    Array array;

    int num = array[5];
    cout << num << endl;

    array[11];

    array[6] = 100;//作为左值
    cout << array[6] << endl;

    return 0;
}
