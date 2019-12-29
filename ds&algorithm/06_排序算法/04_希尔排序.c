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
  int gap = length;
  while(gap>1)
  {
    gap = gap/3 + 1;//缩小增量的算法公式，最后一次gap必为1
    //步距为gap时，那么就一共有gap组序列需要排序
    for(int i = 0; i < gap; i++)
    {
      /*对每一组序列进行插入排序*/
      //序列中无序的部分,i+gap中i是每一个序列的第一个数值，i+gap是每一个序列中第二个数值
      //j+=gap是指每一个序列中下一个数值
      for(int j = i + gap; j < length; j+=gap)
      {
        for(int k = j; k > 0; k-=gap)
        {
          if(arr[k]<arr[k-gap])
          {
            int temp = arr[k];
            arr[k] = arr[k-gap];
            arr[k-gap] = temp;
          }
          else
          {
            break;
          }
        }
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
