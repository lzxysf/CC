#include <stdio.h>
#include "cJSON.h"
#include <string.h>
#include <stdlib.h>

int main()
{
  
  char src = "{\"employees\": 2}";

  // int size = strlen(src) + 1;
  // printf("%d\r\n", size);
  // char* result = malloc(size);

  // memcpy(result, src, 4000);
  // memcpy(result + 4000, src + 4000, size - 4000);
  // printf("%s\r\n", src);

  cJSON* root = NULL;
  root = cJSON_Parse(src);

  return 0;
}
