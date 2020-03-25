#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntop

#define MAXLINE 80

/*
在客户端中接收服务端发送的广播数据。
此客户端可在局域网中多个主机上运行，都会收到服务端广播的数据
*/

int main()
{
	int sockfd;//客户端的端口描述符
	struct sockaddr_in client_addr;//客户端的IPV4地址结构体
	
	//1、创建udp的socket
	if((sockfd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
		printf("create socket error\r\n");
		exit(0);
	}
	
	//2、构造客户端数据接收地址
	bzero(&client_addr, sizeof(struct sockaddr_in));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(8000);
	client_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//3、绑定socket和客户端数据接收地址
	bind(sockfd, (struct sockaddr*)&client_addr, sizeof(struct sockaddr));
	
	//4、接收广播数据
	while(1)
	{
		char buffer[MAXLINE]={0};
		int num = recv(sockfd, buffer, sizeof(buffer), 0);
		printf("recv data is %s\r\n", buffer);
	}
	
	//5、关闭连接
	close(sockfd);
		
	printf("end");
	return 0;
}
