#include <stdio.h>

/*
r	打开一个已有的文本文件，允许读取文件。
w	打开一个文本文件，允许写入文件。如果文件不存在，则会创建一个新文件。在这里，您的程序会从文件的开头写入内容。如果文件存在，则该会被截断为零长度，重新写入。
a	打开一个文本文件，以追加模式写入文件。如果文件不存在，则会创建一个新文件。在这里，您的程序会在已有的文件内容中追加内容。
r+	打开一个文本文件，允许读写文件。
w+	打开一个文本文件，允许读写文件。如果文件已存在，则文件会被截断为零长度，如果文件不存在，则会创建一个新文件。
a+	打开一个文本文件，允许读写文件。如果文件不存在，则会创建一个新文件。读取会从文件的开头开始，写入则只能是追加模式。

总结：
以r和r+方式打开的文件，无法在没有指定文件的情况下创建文件，其它的都可以在没有指定文件情况下创建文件
以w和w+方式打开的文件，文件会被截断为0长度，而不会覆盖写，无论你打开文件是为了读还是写
以a和a+方式打开的文件，都会在文件末尾插入，即使使用fseek移动了文件指针位置
*/

int main()
{
  FILE* fp = fopen("./test.txt","w+");
  fprintf(fp, "hello");
  fclose(fp);

  return 0;
}
