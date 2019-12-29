#include <stdio.h>

/*
environ是定义了的环境变量字符串数组，使用时需要声明extern char ** environ;
NULL作为哨兵结尾
*/

/*
常见环境变量
按照惯例，环境变量字符串都是name=value这样的形式，大多数name由大写字母加下划线组成，一般把name的部分叫做环境变量，value的部分则是环境变量的值。环境变量定义了进程的运行环境，一些比较重要的环境变量的含义如下：

PATH
可执行文件的搜索路径。ls命令也是一个程序，执行它不需要提供完整的路径名/bin/ls，然而通常我们执行当前目录下的程序a.out却需要提供完整的路径名./a.out，这是因为PATH环境变量的值里面包含了ls命令所在的目录/bin，却不包含a.out所在的目录。PATH环境变量的值可以包含多个目录，用:号隔开。在Shell中用echo命令可以查看这个环境变量的值：
$ echo $PATH

SHELL
当前Shell，它的值通常是/bin/bash。
$ echo $SEHLL

TERM
当前终端类型，在图形界面终端下它的值通常是xterm。xterm是一个X Window System上的终端模拟器，用来提供多个独立的SHELL输入输出。终端类型决定了一些程序的输出显示方式，比如图形界面终端可以显示汉字，而字符终端一般不行。
$ echo $TERM

LANG
语言和locale，决定了字符编码以及时间、货币等信息的显示格式。
$ echo $LANG

HOME
当前用户主目录的路径，很多程序需要在主目录下保存配置文件，使得每个用户在运行该程序时都有自己的一套配置。
$ echo $HOME
*/

extern char** environ;

int main()
{
  for(int i = 0; environ[i] != NULL; i++)
  {
    printf("%s\r\n", environ[i]);
  }
  return 0;
}
