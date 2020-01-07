#include <iostream>

using namespace std;

template <class T1, class T2>
class Pair
{
public:
	Pair(T1 key, T2 value):key(key), value(value){}
	bool operator<(const Pair<T1,T2>& p) const
	{
		return key < p.key;
	}
	T1 key;
	T2 value;
};

int main()
{
	Pair<string, int> student("Tom", 20);
	cout << student.key << " " << student.value << endl;
	return 0;
}
