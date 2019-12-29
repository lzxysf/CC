#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

//双指针，两个指针分别指向两个队列的开头
//比较两个指针指向数的大小，把较小的数放到新的队列中，对应的指针+1
//直到某一序列达到了末尾
//如果此时还有一个指针没有没有到末尾，把这个指针后边的数依次放到新队列中即可
vector<int> twosortedseq2onesortedque(vector<int>& v1, vector<int>& v2)
{
  vector<int> myv;
  int size1 = v1.size();
  int size2 = v2.size();
  int i = 0;//指向v1的第一个元素
  int j = 0;//指向v2的第一个元素
  while(i < size1 && j < size2)
  {
    if(v1[i] < v2[j])
    {
      myv.push_back(v1[i]);
      i++;
    }
    else
    {
      myv.push_back(v2[j]);
      j++;
    }
  }
  while(i < size1)
  {
    myv.push_back(v1[i]);
    i++;
  }
  while(j < size2)
  {
    myv.push_back(v2[j]);
    j++;
  }
  return myv;
}

//插入法，将两个序列直接拼接成一个序列，将第一个序列部分当作最终的有序序列，将后一个序列部分插入到第一个序列中
vector<int> twosortedseq2onesortedque_2(vector<int>& v1, vector<int>& v2)
{ 
  int size1 = v1.size();
  int size2 = v2.size();
  vector<int> v3;
  v3.resize(size1+size2);
  merge(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());

  for(int i = size1; i < size1 + size2; i++)
  {
      for(int j = i; j >= 0; j--)
      {
        if(v3[j] < v3[j-1])
        {
            int temp = v3[j];
            v3[j] = v3[j-1];
            v3[j-1] = temp;
        }
        else
        {
            break;
        }
      }
  }

  return v3;
}

int main()
{
  vector<int> v1, v2, v3;
  v1.push_back(2);
  v1.push_back(5);
  v1.push_back(11);
  v1.push_back(17);

  v2.push_back(9);
  v2.push_back(10);
  v2.push_back(17);
  v2.push_back(22);
  v2.push_back(28);

  v3 = twosortedseq2onesortedque_2(v1, v2);
  
  for(int i = 0; i < v3.size(); i++)
  {
    printf("%d ", v3[i]);
  }
  printf("\r\n");

  return 0;
}
