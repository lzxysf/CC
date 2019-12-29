#include <stdio.h>

int strStr(char * haystack, char * needle){
    int position = -1;
    while(*haystack)
    {
        position++;
        char* temp1 = haystack;
        char* temp2 = needle;
        while(*temp1 && *temp2 && *temp1 == *temp2)
        {
            temp1++;
            temp2++;
        }
        if(*temp2 == '\0')
        {
            return position;
        }

        haystack++;
    }
    return -1;
}

int main()
{
  int ret = strStr("hello", "lo");
  printf("%d", ret);

  return 0;
}
