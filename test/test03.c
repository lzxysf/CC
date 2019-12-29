#include <stdio.h>
#include <string.h>

/*
memcpy的用法
*/

int main()
{
  int nums[5] = {1,1,3,4,5};
  int nums_c[4] = {9,9,9,10};
  memcpy(nums, nums_c, sizeof(int)*4);//此处必须是sizeof(int)*4而不是4
  for(int i = 0; i < 4; i++)
  {
    printf("%d\t", nums[i]);
  }
  return 0;
}
