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
	int maxfd;
	int maxi;
	int num;
	int sockfd;
	int clifd[FD_SETSIZE];
	struct sockaddr_in server_addr;
	char buffer[MAXLINE];
	
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
	
	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(sockfd, &read_fds);
	
	while(1)
	{			
		num = select(maxfd+1, &read_fds, NULL, NULL, NULL);
		if(num < 0)
		{
			perror("select error\r\n");
			exit(1);
		}
		
		if(FD_ISSET(sockfd, &read_fds))
		{	
			struct sockaddr_in client_addr;
			char client_addr_str[INET_ADDRSTRLEN];
			int port;
			int size;
			
			for(int i = 0; i < FD_SETSIZE; i++)
			{
				if(clifd[i]<0)
				{
					maxi = i;
					break;
				}
			}
			
			size = sizeof(struct sockaddr);
			clifd[maxi] = accept(sockfd, (struct sockaddr*)&client_addr, &size);
			if(clifd[maxi]>maxfd)
			{
				maxfd = clifd[maxi];	
			}
			FD_SET(clifd[maxi], &read_fds);

			inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_str, sizeof(client_addr_str));
                        port = ntohs(client_addr.sin_port); 
                        printf("a client connected, ip is %s, port is %d\r\n", client_addr_str, port);
		}
		else
		{
			printf("有数据到达\r\n");
			for(int i = 0; i < maxi; i++)
			{
				if(FD_ISSET(clifd[i], &read_fds))
				{
					read(clifd[i], buffer, sizeof(buffer));
					printf("receive data is %s\r\n", buffer);
				}
			}
		}
	}		
		
	return 0;
}
