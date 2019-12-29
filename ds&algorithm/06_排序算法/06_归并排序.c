#include <stdio.h>
#include <string.h>

//merge，将两个有序的序列合并成一个有序的序列
void _merge(int* arr, int begin1, int end1, int begin2, int end2)
{
  int size = end2-begin1+1;
  int temp[size];
  int k = 0;
  int i = begin1;
  int j = begin2;

  while(i <= end1 && j <= end2)
  {
    if(arr[i] < arr[j])
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
  memcpy(arr+begin1, temp, size*sizeof(int));
}

void merge_sort(int* arr, int begin, int end)
{
  if(begin == end)
  {
    return;
  }
  int mid = (begin+end)/2;
  merge_sort(arr, begin, mid);
  merge_sort(arr, mid+1, end);
  _merge(arr, begin, mid, mid+1, end);
}


int main()
{
  int a[10] = {1, 4, 10, 2, 5, 9, 11, 21, 0, 5};
  merge_sort(a, 0, 9);

  for(int i = 0; i < 10; i++)
  {
    printf("%d ", a[i]);
  }

  return 0;
}
