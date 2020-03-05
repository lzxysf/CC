/*
借助fcntl函数来实现锁机制。操作文件的进程没有获得锁时，可以打开，但无法执行read、write操作。

fcntl函数：获取、设置文件访问控制属性。

int fcntl(int fd, int cmd, ...arg);

参数2：
    F_SETLK
    F_SETLKW
    F_GETLK

参数3：
    struct flock{
	short l_type;		锁的类型：F_RDLCK、F_WRLCK、F_UNLCK
	short l_whence;		偏移位置：SEEK_SET、SEEK_CUR、SEEK_END
	short l_start;		起始偏移
	short l_len;		长度：0表示整个文件加锁
	short l_pid;		持有该锁的进程ID（F_GETLK only）
	...
    };
*/

/*
文件锁遵循“读共享、写独占”的特性。但是如果进程不加锁直接操作文件依然可以访问成功，但数据势必会出现混乱。

多进程中使用文件锁，但多线程中不可以使用文件锁。
因为多线程间共享文件描述符，而给文件加锁是通过修改文件描述符所指向的文件结构体中的成员变量来实现的。因此多线程中无法使用文件锁。
*/

/*
示例程序：
文件锁写、文件锁读
当文件锁写时， 文件锁读会阻塞

gcc 15_文件锁_写.c -o fileread
gcc 16_文件锁_读.c -o filewrite
./filewrite
./fileread

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int fd = 0;
	struct flock f_lock;
	
	fd = open("./test", O_RDWR|O_TRUNC|O_CREAT, 0664);
		
	f_lock.l_type = F_WRLCK;
	f_lock.l_whence = SEEK_SET;
	f_lock.l_start = 0;
	f_lock.l_len = 0;

	fcntl(fd, F_SETLKW, &f_lock);
	printf("get flock\r\n");
	sleep(20);

	f_lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLKW, &f_lock);
	printf("un flock\r\n");

	close(fd);
	return 0;
}

