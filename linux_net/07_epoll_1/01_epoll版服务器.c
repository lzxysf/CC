/*
select：采用轮询的方式查询文件描述符是否就绪，且最多只能监测1024个文件描述符，当监测的文件描述符较多时对性能会有影响
poll：和select相似，同样采用轮询的方式查询文件描述符是否就绪，不同点在于它能监测的文件描述符数量没有上限
epoll：和select、poll的原理不同，不用轮询、效率较高，它的时间复杂度是O(1),而select和poll的时间复杂度都是O(n),
每当有文件描述符就绪时IO事件会主动通知给我们，而不是轮询IO，因此它是事件驱动的。
epoll可以高效地处理数以百万计的socket句柄。
*/

/*
头文件 #include <sys/epoll.h>
epoll相关的共有三个函数epoll_create、epoll_ctl、epoll_wait
epoll_create创建一个epoll实例，epoll_ctl将要监控的文件描述符设置进epoll实例，epoll_wait等待文件描述符就绪

int epoll_create(int size);
创建一个epoll实例，其中size是可以监控的文件描述符的总数，但是现在这个参数已经废弃，系统会根据你传进去的实际的文件描述符个数分配内存空间。
返回值是一个文件句柄，因此它会占用一个fd值，使用完epoll后应该用close关闭这个文件句柄，否则会造成内存泄漏。
调用epoll_create会在内部创建一个红黑树，

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
将要监控的文件描述符设置进epoll实例中
第一个参数epfd为epoll_create创建的epoll实例
第二个参数op表示动作，有以下三种取值
	EPOLL_CTL_ADD	注册新的fd到epfd
	EPOLL_CTL_MOD	修改已经注册的fd的监听事件
	EPOLL_CTL_DEL	从epfd中删除一个fd
第三个参数fd为要添加、修改或删除的文件描述符
第四个参数为epoll_event结构体指针。第三个参数fd为key，第四个参数epoll_event指针为value，在epoll_wait中当fd就绪时epoll_event指针会返回
struct epoll_event {
	uint32_t events;	//epoll events
	epoll_data_t data;	//user data variable
}
typedef union epoll_data {
	void *ptr;
	int fd;
	uint32_t u32;
	uint64_t u64;
}epoll_data_t;
其中epoll_data是一个共用体，可以传入fd，也可以传入一个32位或64位数值，
当然也可以传入指针，这个指针可以是一个函数指针，这样就可以在某个文件描述符就绪后调用这个函数进行处理
这个指针也可以是一个结构体指针，这样就可以传递更多信息

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
等待文件描述符就绪
第一个参数epfd为epoll_create创建的epoll实例
第二个参数events为epoll_event结构体数组（数组中的元素为epoll_ctl中传入的epoll_event结构体指针），当函数返回时events是传出参数
第三个参数maxevents为最大监控的文件描述符的个数
第四个参数timeout为阻塞等待的时间，为0不等待，为-1无限等待
返回值大于0为就绪的文件描述符个数，等于0为等待超时，小于0为出错
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define MAXNUM 10000
#define MAXLINE 1024

int main()
{
	int epfd; //创建的epoll句柄
	struct epoll_event event; //用于注册事件
	struct epoll_event events[1000]; //用于回传需要处理的事件
	int sockfd; //服务端的文件描述符
	struct sockaddr_in server_addr; //服务端地址

	epfd = epoll_create(1000);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd==-1)
	{
		perror("socket error!\r\n");
		exit(0);
	}

	//添加要监控的文件描述符
	event.events = EPOLLIN;
	event.data.fd = sockfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));

	listen(sockfd, 5);
	printf("正在监听端口...\r\n");
	
	while(1)
	{
		int nready; //表示有几个文件描述符就绪

		nready = epoll_wait(epfd, events, 1000, -1);

		if(nready < 0)
		{
			perror("epoll error\r\n");
			continue;
		}
		else if(nready == 0)
		{
			printf("epoll timeout...\r\n");
			continue;
		}

		for(int i = 0; i < nready; i++)
		{
			//文件描述符就绪且有数据可读
			if(events[i].events & EPOLLIN)
			{
				//sockfd文件描述符有新客户端连接
				if(events[i].data.fd == sockfd)
				{
					struct sockaddr_in client_addr;
					char client_addr_str[INET_ADDRSTRLEN];

					int size = sizeof(struct sockaddr);
					int client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &size);
					inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_str, sizeof(client_addr_str));
					int port = ntohs(client_addr.sin_port); 
					printf("a client connected, ip is %s, port is %d\r\n", client_addr_str, port);

					if(client_fd > 0)
					{
						event.events = EPOLLIN;
						event.data.fd = client_fd;
						epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &event);
					}
					if(--nready == 0)
					{
						continue;
					}
				}
				//接收到客户端发来的数据
				else
				{
					char buffer[MAXLINE]={0}; //接收客户端数据缓冲区，记得初始化清零
					int n = read(events[i].data.fd, buffer, sizeof(buffer));
					//当read的返回值为0时表示客户端断开
					if(n==0)
					{
						close(events[i].data.fd);
						printf("客户端关闭\r\n");
						epoll_ctl(epfd, EPOLL_CTL_ADD, events[i].data.fd, NULL);
					}
					else
					{
						printf("receive data is %s\r\n", buffer);
					}

					if(--nready == 0)
					{
						break;
					}
				}
			}
		}
	}	

	close(sockfd);
	close(epfd);
	return 0;
}
