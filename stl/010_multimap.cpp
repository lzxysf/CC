#include <iostream>
#include <string.h>
#include <map>

using namespace std;

void test1()
{
  multimap<int,string> mymulmap;
  mymulmap.insert(make_pair(1, "hello"));
  mymulmap.insert(make_pair(2, "world"));
  mymulmap.insert(make_pair(2, "WORLD"));

  cout << mymulmap.size() << endl;
  for(multimap<int,string>::iterator it = mymulmap.begin(); it != mymulmap.end(); it++)
  {
    cout << it->first << ":" << it->second << endl;
  }
}

int main()
{
  test1();
  return 0;
}