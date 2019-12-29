#include <stdio.h>

void print(int* arr, int length)
{
  for(int i = 0; i < length; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\r\n");
}

void bubble_sort(int* arr, int length)
{
  for(int j = 0; j < length-1; j++)
  {
    for(int i = 0; i < length-1-j; i++)
    {
      if(arr[i] > arr[i+1])
      {
        int temp = arr[i];
        arr[i] = arr[i+1];
        arr[i+1] = temp;
      }
    }
  }
}

int main()
{
  int arr[10] = {2, 4, 3, 21, 45, 34, 35, 11, 12, 22};
  bubble_sort(arr, sizeof(arr)/sizeof(int));
  print(arr, sizeof(arr)/sizeof(int));
  return 0;
}
