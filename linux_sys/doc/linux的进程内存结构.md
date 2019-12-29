  Linux操作系统采用虚拟内存管理技术，使得每个进程都有各自互不干涉的进程地址空间。该地址空间是大小为4GB的线性虚拟空间，用户所看到和接触到的都是该虚拟地址，无法看到实际的物理内存地址。利用这种虚拟地址不但能起到保护操作系统的效果（用户不能直接访问物理内存），更重要的是，用户程序可以使用比实际物理内存更大的地址空间。

　　4GB的进程地址空间会被分成两个部分：用户空间与内核空间。用户地址空间是从0到3GB（0xc0000000),内核地址空间占据3GB到4GB。用户进程在通常情况下只能访问用户空间的虚拟地址，不能访问内核空间的虚拟地址。

![linux进程内存结构](https://dev.tencent.com/u/lzxysf001/p/CC/git/raw/master/zothers/linux%E8%BF%9B%E7%A8%8B%E5%86%85%E5%AD%98%E7%BB%93%E6%9E%84.png)


在linux中使用“size 可执行文件”可以查看程序的组成
02_文件目录 git:(master) ✗ size a.out
   text    data     bss     dec     hex filename
   1483     560       8    2051     803 a.out

内核空间
gap  gap间隙使堆和栈起始地址随机不固定
栈段  (向下增长)
mmap映射区 (向上增长)(映射其它库的相关信息)
堆段  (向上增长)
gap  gap间隙使堆和栈起始地址随机不固定，好处是更安全，防止出现安全漏洞
bss  bss段，存放未经初始化的全局变量和静态变量
data 数据段，存放已经初始化的全局变量和静态变量
text 代码段，存放代码

执行程序时,系统首先在内核空间中创建一个进程,为这个进程申请PCB(进程控制块 task_struct)用于管理整个进程的所有资源,其中mm_struct成员用来管理与当前进程相关的所有内存资源
![mm_struct结构](https://dev.tencent.com/u/lzxysf001/p/CC/git/raw/master/zothers/mm_struct.jpg)
