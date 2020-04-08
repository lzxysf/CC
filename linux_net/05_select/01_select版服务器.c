/*
tcp通信中，accept和read是阻塞的，这样就需要开启线程或进程去处理，
但是当连接过多时，就需要开启很多个线程或进程，这样会浪费很多系统资源，而且可扩展性很差
因此有了select，它可以阻塞监控多个文件描述符的读、写、异常数据的到达，当有文件描述符就绪就会解除阻塞。
*/

/*
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

nfds--所传入的最大文件描述符+1
readfds--可读文件描述符集合
writefds--可写文件描述符集合
exceptfds--异常文件描述符集合
timeout--阻塞等待时间，传NULL时为无限期等待

readfds、writefds、exceptfds、timeout即是传入参数，又是传出参数，当select运行结束后它们就作为传出参数传输。
因此每次执行select函数都应该重新传入构造好的readfds、writefds、exceptfds、timeout

成功时返回值为就绪的文件描述符总数，失败返回-1并设置errno，超时则返回值为0

fd_set是文件描述符集合
FD_ZERO(fd_set *set);			初始化文件描述符集合
FD_SET(int fd, fd_set *set);	将一个文件描述符添加到集合
FD_CLR(int fd, fd_set *set);	将一个文件描述符从集合中删除
FD_ISSET(int fd, fd_set *set);	判断文件描述符集合中某个文件描述符是否就绪
*/

/*
select的原理即多路IO复用，它不会阻塞在某个文件描述符。和select相同的有poll和epoll，它们的区别如下
select：采用轮询的方式查询文件描述符是否就绪，且最多只能监测1024个文件描述符，当监测的文件描述符较多时对性能会有影响
poll：和select相似，同样采用轮询的方式查询文件描述符是否就绪，不同点在于它能监测的文件描述符数量没有上限
epoll：和select、poll的原理不同，不用轮询、效率较高，它的时间复杂度是O(1),而select和poll的时间复杂度都是O(n),
每当有文件描述符就绪时IO事件会主动通知给我们，而不是轮询IO，因此它是事件驱动的。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define MAXLINE 1024

int main()
{
	int maxfd; //所有文件描述符中值最大的
	int clifd[FD_SETSIZE]; //accept到的客户端的文件描述符数组，FD_SETSIZE为1024，因为select最多可以监听1024个文件描述符
	fd_set read_fds; //需要被监听是否可读的文件描述符的集合，传入select
	fd_set allset; //read_fds作为传入参数同时也是传出参数，它会被修改，因此每次select前将allset重新赋值给read_fds
	int maxi = -1; //上述数组中的存放有效的客户端描述符的最大序号，用于遍历查找哪个文件描述符有数据到达
	int num; //表示num个文件描述符有数据到达
	int sockfd; //服务端的文件描述符
	struct sockaddr_in server_addr; //服务端地址
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd==-1)
	{
		perror("socket error!\r\n");
		exit(0);
	}	

	maxfd = sockfd;
	//初始化客户端文件描述符为-1
	for(int i = 0; i < FD_SETSIZE; i++)
	{
		clifd[i] = -1;
	}
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));

	listen(sockfd, 5);
	printf("正在监听端口...\r\n");
	
	FD_ZERO(&allset);
	FD_SET(sockfd, &allset);
	
	while(1)
	{			
		read_fds = allset;
		num = select(maxfd+1, &read_fds, NULL, NULL, NULL);
		if(num < 0)
		{
			perror("select error\r\n");
			exit(1);
		}
		
		//sockfd文件描述符有数据可读出，即有新客户端连接
		if(FD_ISSET(sockfd, &read_fds))
		{	
			struct sockaddr_in client_addr;
			char client_addr_str[INET_ADDRSTRLEN];
			int port;
			int size;
			
			//寻找数组最前面的未被占用的文件描述符，并更新maxi
			for(int i = 0; i < FD_SETSIZE; i++)
			{
				if(clifd[i]<0)
				{
					if(i > maxi)
					{
						maxi = i;
					}
					break;
				}
			}
			
			size = sizeof(struct sockaddr);
			clifd[maxi] = accept(sockfd, (struct sockaddr*)&client_addr, &size);
			if(clifd[maxi]>maxfd)
			{
				//有了更大的文件描述符数值，更新maxfd
				maxfd = clifd[maxi];
			}
			//将新的客户端文件描述符加入到allset，用于接下来的select
			FD_SET(clifd[maxi], &allset);

			inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_str, sizeof(client_addr_str));
                        port = ntohs(client_addr.sin_port); 
                        printf("a client connected, ip is %s, port is %d\r\n", client_addr_str, port);
		}
		else
		{	
			//查找哪个客户端有数据到达
			for(int i = 0; i <= maxi; i++)
			{
				if(FD_ISSET(clifd[i], &read_fds))
				{	
					char buffer[MAXLINE]={0}; //接收客户端数据缓冲区，记得初始化清零
					int n = read(clifd[i], buffer, sizeof(buffer));
					//当read的返回值为0时表示客户端断开
					if(n==0)
					{
						close(clifd[i]);
						printf("客户端关闭\r\n");
						FD_CLR(clifd[i], &allset);
						clifd[i] = -1;
					}
					else
					{
						printf("receive data is %s\r\n", buffer);
					}
				}
			}
		}
	}	
	return 0;
}
