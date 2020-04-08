/*
select：采用轮询的方式查询文件描述符是否就绪，且最多只能监测1024个文件描述符，当监测的文件描述符较多时对性能会有影响
poll：和select相似，同样采用轮询的方式查询文件描述符是否就绪，不同点在于它能监测的文件描述符数量没有上限
epoll：和select、poll的原理不同，不用轮询、效率较高，它的时间复杂度是O(1),而select和poll的时间复杂度都是O(n),
每当有文件描述符就绪时IO事件会主动通知给我们，而不是轮询IO，因此它是事件驱动的。
epoll可以高效地处理数以百万计的socket句柄。
*/

/*
EPOLLET触发、非阻塞模型服务端

非阻塞IO、边沿触发模式、循环读IO

非阻塞read时，返回值为-1且errno为EAGAIN时，表示已经读到数据流的末尾了
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>

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
						//设置为非阻塞IO
						int flag = fcntl(client_fd, F_GETFL);
						flag |= O_NONBLOCK;
						fcntl(client_fd, F_SETFL, flag);
						//设置为边沿触发
						event.events = EPOLLIN | EPOLLET; //边沿触发
						event.data.fd = client_fd;
						epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &event);
					}
				}
				//接收到客户端发来的数据
				else
				{
					//接收客户端数据缓冲区，记得初始化清零
					//在MAXLINE很大时，由于buffer已经清0，所以有效数据末尾也是\0
					//但是在MAXLINE很小时，数据填满了buffer，每次接收到的数据末尾没有\0，此时如果要正常打印出，需要在buffer后手动添加\0
					char buffer[MAXLINE]={0};

					while(1)
					{
						int n = read(events[i].data.fd, buffer, sizeof(buffer));
						//有实际数据到达
						if(n > 0)
						{
							char buf[n+1];
							strcpy(buf, buffer);
							buf[n] = '\0';
							printf("receive data is %s\r\n", buf);
						}
						//当read的返回值为0时表示客户端断开
						else if(n == 0)
						{
							close(events[i].data.fd);
							printf("客户端关闭\r\n");
							epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
							break;
						}
						//读取到数据末尾，无数据可读了
						else if(n < 0 && errno == EAGAIN)
						{
							break;
						}
					}
				}
			}
		}
		usleep(1000);
	}	

	close(sockfd);
	close(epfd);
	return 0;
}
