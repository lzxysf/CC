#include <stdio.h>

void quick_sort(int* arr, int start, int end)
{
  if(start > end)
  {
    return;
  }

  int i = start;
  int j = end;

  int tmp = arr[i];
  while(i < j)
  {
    while(arr[j] >= tmp && i < j)
    {
      j--;
    }
    arr[i] = arr[j];

    while(arr[i] <= tmp && i < j)
    {
      i++;
    }
    arr[j] = arr[i];
  }
  arr[i] = tmp;

  quick_sort(arr, start, i-1);
  quick_sort(arr, i+1, end);
}

int main()
{
  int arr[] = {3, 5, 1, 12, 21, 0, 2, 9, 3, 6};
  int len = sizeof(arr)/sizeof(int);

  quick_sort(arr, 0, len-1);

  for(int i = 0; i < len; i++)
  {
    printf("%d ", arr[i]);
  }

  return 0;
}