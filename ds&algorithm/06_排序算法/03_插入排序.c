#include <stdio.h>

void print(int* arr, int length)
{
  for(int i = 0; i < length; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\r\n");
}

void insert_sort(int* arr, int length)
{
  //从第二个到最后一个元素，插入到前面组成的有序数组的合适位置
  for(int i = 1; i < length; i++)
  {
    for(int j = i; j >= 0; j--)
    {
      if(arr[j] < arr[j-1])
      {
        int temp = arr[j];
        arr[j] = arr[j-1];
        arr[j-1] = temp;
      }
      else
      {
        break;
      }
    }
  }
}

int main()
{
  int arr[10] = {2, 4, 3, 21, 45, 34, 35, 11, 12, 22};
  insert_sort(arr, sizeof(arr)/sizeof(int));
  print(arr, sizeof(arr)/sizeof(int));
  return 0;
}
