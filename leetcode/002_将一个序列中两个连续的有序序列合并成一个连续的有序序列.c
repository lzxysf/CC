#include <stdio.h>
#include <string.h>

/*比如，将序列[3,5,1,10, 6,8,11,12, 2,5,9,20, 3,23,31]中两个连续的有序序列[6,8,11,12]和[2,5,9,20]合并成一个有序序列*/

//还是用双指针的方法进行排序，两个指针分别指向两个有序序列的开头
void _merge(int* arr, int begin1, int end1, int begin2, int end2)
{
  int temp[end2-begin1+1];
  int k = 0;
  int i = begin1;
  int j = begin2;
  while(i <= end1 && j <= end2)
  {
    if(arr[i]<arr[j])
    {
      temp[k++] = arr[i];
      i++;
    }
    else
    {
      temp[k++] = arr[j];
      j++;
    }
  }
  while(i <= end1)
  {
    temp[k++] = arr[i];
    i++;
  }
  while(j <= end2)
  {
    temp[k++] = arr[j];
    j++;
  }

  memcpy(arr + begin1, temp, (end2 - begin1 + 1)*sizeof(int));
}

int main()
{
  int arr[15] = {3,5,1,10,6,8,11,12,2,5,9,20,3,23,31};
  _merge(arr, 4, 7, 8, 11);
  for(int i = 0; i < 15; i++)
  {
    printf("%d ", arr[i]);
  }
  return 0;
}
