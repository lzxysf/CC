#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntop

#define MAXLINE 80

int main()
{
	int sockfd;//服务端的端口描述符
	struct sockaddr_in server_addr;//服务端的IPV4地址结构体
	
	//1、创建udp的socket
	if((sockfd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
		printf("create socket error\r\n");
		exit(0);
	}
	
	//2、构造服务端地址
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//3、绑定socket和服务端地址
	bind(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));

	printf("udpserver running...\r\n");
		
	while(1)
	{
		struct sockaddr_in client_addr;
		char client_addr_str[INET_ADDRSTRLEN];
		int addrlen = sizeof(struct sockaddr);
		char buffer[MAXLINE]={0};

		//int num = recv(sockfd, buffer, sizeof(buffer), 0);
		int num = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addrlen);
		inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_str, sizeof(client_addr_str));
		int port = ntohs(client_addr.sin_port); 
		printf("recv data is %s, come from %s, port %d\r\n", buffer, client_addr_str, port);
	}
	
	//6、关闭连接
	close(sockfd);
		
	printf("end");
	return 0;
}
