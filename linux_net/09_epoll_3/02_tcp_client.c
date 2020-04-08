#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int sockfd;//客户端的端口描述符
	struct sockaddr_in server_addr;
	char buffer[256] = "Hello,I am client";
	char recv_buffer[256];
	
	//1、创建socket
	if((sockfd=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		printf("create socket error\r\n");
		exit(0);
	}
	
	//2、构造连接的服务器的地址
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//3、连接服务器
	if(connect(sockfd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr)) == -1)
	{
		printf("connect error\r\n");
		exit(1);
	}
	printf("连接到服务器成功\r\n");
	
	//4、发送数据到服务器
	while(1)
	{
		char buf[256]={0};
		static int count = 0;
		count++;
		if(count>5)
		{
			break;
		}

		sprintf(buf, "%s %d", buffer, count);

		send(sockfd, buf, strlen(buf), 0);
		sleep(1);
	}		
	//5、关闭连接
	close(sockfd);
		
	printf("end\r\n");
	return 0;
}
