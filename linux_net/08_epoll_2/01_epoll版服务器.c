/*
select：采用轮询的方式查询文件描述符是否就绪，且最多只能监测1024个文件描述符，当监测的文件描述符较多时对性能会有影响
poll：和select相似，同样采用轮询的方式查询文件描述符是否就绪，不同点在于它能监测的文件描述符数量没有上限
epoll：和select、poll的原理不同，不用轮询、效率较高，它的时间复杂度是O(1),而select和poll的时间复杂度都是O(n),
每当有文件描述符就绪时IO事件会主动通知给我们，而不是轮询IO，因此它是事件驱动的。
*/

/*
int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

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

在epoll_event中data是一个共用体，在epoll_1例子中传入的是fd，
在本例中针对客户端的文件描述符传入一个自定义的结构体指针event_ptr*

struct event_ptr{
	int fd;						//是哪个文件描述符
	void (*func)(void*);		//使用哪个函数处理这个IO事件
};

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

int epfd; //创建的epoll句柄

struct event_ptr{
	int fd;					//是哪个文件描述符
	void (*func)(void*);		//使用哪个函数处理这个IO事件
};

void process1(void* ptr)
{
	printf("处理函数1");
	char buffer[MAXLINE]={0}; //接收客户端数据缓冲区，记得初始化清零
	struct event_ptr* eptr = (struct event_ptr*)ptr;

	int n = read(eptr->fd, buffer, sizeof(buffer));
	//当read的返回值为0时表示客户端断开
	if(n==0)
	{
		close(eptr->fd);
		printf("客户端关闭\r\n");
		epoll_ctl(epfd, EPOLL_CTL_DEL, eptr->fd, NULL);
		free(eptr);
	}
	else
	{
		printf("receive data is %s\r\n", buffer);
	}
}

void process2(void* ptr)
{
	printf("处理函数2：有数据到达\r\n");
}

int main()
{
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

		printf("test\r\n");

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
						//event.events = EPOLLIN | EPOLLET;
						struct event_ptr* ptr = (struct event_ptr*)malloc(sizeof(struct event_ptr));
						ptr->fd = client_fd;
						ptr->func = process1;
						//ptr->func = process2;
						event.data.ptr = ptr;
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
					struct event_ptr* ptr = (struct event_ptr*)events[i].data.ptr;
					ptr->func(ptr);
				}
			}
		}
	}	

	close(sockfd);
	close(epfd);
	return 0;
}
