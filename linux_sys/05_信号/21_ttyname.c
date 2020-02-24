#include <stdio.h>
#include <unistd.h>

int main()
{
	char* str = ttyname(0);
	printf("%s\r\n", str);
	
	char* str1 = ttyname(1);
	printf("%s\r\n", str1);

	char* str2 = ttyname(2);
	printf("%s\r\n", str2);

	return 0;
}

