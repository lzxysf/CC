#include <stdio.h>

void print(int* arr, int length)
{
  for(int i = 0; i < length; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\r\n");
}

//选择排序，即每次从整个数组中选择最小（或最大）的元素放在数组的第一位，然后在剩余的元素中选择最大的元素放在数组第二位，以此类推
void select_sort(int* arr, int length)
{
  //第一次从0开始到最后选择最小的数，最后一次从倒数第二个数开始到最后选择最小的数
  for(int j = 0; j < length - 1; j++)
  {
    //保存本次比较过程中，最小值的位置
    int min = j; 
    for(int i = j; i < length; i++)
    {
      if(arr[i] < arr[min])
      {
        min = i;
      }
    }
    if(j != min)
    {
      int temp = arr[j];
      arr[j] = arr[min];
      arr[min] = temp;
    }
  }
}

int main()
{
  int arr[10] = {2, 4, 3, 21, 45, 34, 35, 11, 12, 22};
  select_sort(arr, sizeof(arr)/sizeof(int));
  print(arr, sizeof(arr)/sizeof(int));
  return 0;
}
