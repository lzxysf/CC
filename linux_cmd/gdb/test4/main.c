#include <stdio.h>
#include <string.h>

int endwith(char* str, char* end);

int main()
{
  char* s = "hello world";
  s[strlen(s)+10] = 'o';
  printf("test hello");
}
