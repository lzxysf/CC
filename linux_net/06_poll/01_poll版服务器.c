/*
select：采用轮询的方式查询文件描述符是否就绪，且最多只能监测1024个文件描述符，当监测的文件描述符较多时对性能会有影响
poll：和select相似，同样采用轮询的方式查询文件描述符是否就绪，不同点在于它能监测的文件描述符数量没有上限
epoll：和select、poll的原理不同，不用轮询、效率较高，它的时间复杂度是O(1),而select和poll的时间复杂度都是O(n),
每当有文件描述符就绪时IO事件会主动通知给我们，而不是轮询IO，因此它是事件驱动的。
*/

/*
int poll(struct pollfd *fds, nfds_t nfds, int timeout);

pollfd结构体如下:
struct pollfd{
	int fd;
	short events;
	short revents;
};

第一个参数fds，为一个pollfd结构体数组
第二个参数nfds，为需要监测的文件描述符个数，注意和select不一样，select中传入的是文件描述符中的最大值加1
第三个参数timeout，为阻塞等待时间，单位为毫秒，传入-1为无限等待

返回值：大于0时为就绪文件描述符个数，等于0为到达等待时间，小于0为函数执行出错

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>

#define MAXNUM 10000
#define MAXLINE 1024

int main()
{
	struct pollfd pfd[MAXNUM] = {0}; //pollfd结构体数组
	int nfds; //文件描述符个数
	int maxi; //pollfd数组中存放有效数据的最大下标
	int sockfd; //服务端的文件描述符
	struct sockaddr_in server_addr; //服务端地址
	
	//初始化pollfd数组中fd
	for(int i = 0; i < MAXNUM; i++)
	{
		pfd[i].fd = -1;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd==-1)
	{
		perror("socket error!\r\n");
		exit(0);
	}	
	pfd[0].fd = sockfd;
	pfd[0].events = POLLIN;
	nfds = 1;
	maxi = 0;
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));

	listen(sockfd, 5);
	printf("正在监听端口...\r\n");
	
	while(1)
	{
		struct sockaddr_in client_addr;
		char client_addr_str[INET_ADDRSTRLEN];
		int nready; //表示有几个文件描述符就绪
		int size;
		int port;

		nready = poll(pfd, nfds, 2000);
		if(nready < 0)
		{
			perror("poll error\r\n");
			continue;
		}
		else if(nready == 0)
		{
			printf("poll timeout...\r\n");
			continue;
		}
		
		//sockfd文件描述符有数据可读出，即有新客户端连接
		if(pfd[0].revents & POLLIN)
		{
			size = sizeof(struct sockaddr);
			int client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &size);
			inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_str, sizeof(client_addr_str));
			port = ntohs(client_addr.sin_port); 
			printf("a client connected, ip is %s, port is %d\r\n", client_addr_str, port);

			if(client_fd > 0)
			{
				for(int i = 0; i <= MAXNUM; i++)
				{
					if(pfd[i].fd == -1)
					{
						pfd[i].fd = client_fd;
						pfd[i].events = POLLIN;
						maxi = i > maxi ? i : maxi;
						break;
					}
				}
				nfds++;
			}
			if(--nready == 0)
			{
				continue;
			}
		}

		//客户端有数据到达
		for(int i = 1; i <= maxi; i++)
		{
			if(pfd[i].revents & POLLIN)
			{
				char buffer[MAXLINE]={0}; //接收客户端数据缓冲区，记得初始化清零
				int n = read(pfd[i].fd, buffer, sizeof(buffer));
				//当read的返回值为0时表示客户端断开
				if(n==0)
				{
					close(pfd[i].fd);
					printf("客户端关闭\r\n");
					pfd[i].fd = -1;
					pfd[i].events = 0;
					pfd[i].revents = 0;
					nfds--;
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
	return 0;
}
