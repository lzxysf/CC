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
	fd_set allset;
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
				maxfd = clifd[maxi];
			}
			FD_SET(clifd[maxi], &allset);

			inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_str, sizeof(client_addr_str));
                        port = ntohs(client_addr.sin_port); 
                        printf("a client connected, ip is %s, port is %d\r\n", client_addr_str, port);
		}
		else
		{	
			//查找哪个文件描述符有数据到达
			for(int i = 0; i <= maxi; i++)
			{
				if(FD_ISSET(clifd[i], &read_fds))
				{	
					char buffer[MAXLINE]={0}; //接收客户端数据缓冲区
					int n = read(clifd[i], buffer, sizeof(buffer));
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

