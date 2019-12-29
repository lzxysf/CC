#include <stdio.h>
#include <math.h>
#include <limits.h>

int divide(int dividend, int divisor)
{
  if(dividend==0) return 0;
  if(dividend==divisor) return 1;

  int flag = dividend ^ divisor;
  
  dividend = dividend > 0 ? dividend : -dividend;
  divisor = divisor > 0 ? divisor : -divisor;

  int count = 0;
  int div = divisor;
  int ntimes = 1;
  while(div<<1 < dividend)
  {
    div = div << 1;
    ntimes = ntimes << 1;
  }
  while(dividend>=divisor)
  {
    dividend = dividend - div;
    count += ntimes;
    while(dividend < div)
    {
      div = div >> 1;
      ntimes = ntimes >> 1;
    }
  }
  count = flag > 0 ? count : -count;
  return count;
}

int main()
{
  int ret = divide(2, 2);
  printf("%d", ret);
  return 0;
}
