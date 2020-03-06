#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntop

#define MAXLINE 80 //一次读取客户端的数据长度

int main()
{
	int sockfd;//服务端的端口描述符
	struct sockaddr_in server_addr;//服务端的IPV4地址结构体
	
	//1、创建socket
	if((sockfd=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		printf("create socket error\r\n");
		exit(0);
	}
	
	//2、构造服务端地址
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(3333);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//3、绑定socket和服务端地址
	bind(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	
	//4、监听socket，最多可接受5个客户端连接
	listen(sockfd, 5);
	printf("正在监听端口...\r\n");

	while(1)
	{
		int newfd;//客户端的端口描述符
		struct sockaddr_in client_addr;//客户端的IPV4地址结构体
		int size = sizeof(struct sockaddr);//客户端地址长度
		
		char client_addr_str[INET_ADDRSTRLEN];//客户端地址字符串
		int port;//客户端端口号
		char buffer[MAXLINE];//读取数据缓冲区
		int count = 0;//读取到的数据的实际长度

		//5、等待客户端连接
		newfd = accept(sockfd, (struct sockaddr*)&client_addr, &size);
	
		inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_str, sizeof(client_addr_str));
		port = ntohs(client_addr.sin_port); 
		printf("a client connected, ip is %s, port is %d\r\n", client_addr_str, port);

		count = read(newfd, buffer, sizeof(buffer));		
		printf("读取到的数据为:%s\r\n", buffer);

		for(int i = 0; i < count; i++)
		{
			buffer[i] = toupper(buffer[i]);
		}
		write(newfd, buffer, count);

		close(newfd);	
	}
	
	//6、关闭连接
	close(sockfd);
		
	printf("end");
	return 0;
}
