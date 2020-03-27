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
本实例的功能为:在一个局域网中一个服务端发送广播，多个客户端接收广播
我们可以看出在udp中没有严格的服务端和客户端的限制，不一定是客户端先发数据
本例中由于发送数据的只有一方，我们人为将其定义为服务端
*/

/*
xxx.xxx.xxx.255 直接广播地址。代码中，使用直接广播地址的话，需要绑定本地地址
255.255.255.255 受限广播地址（有限广播地址、本地广播地址），如果只需在本网络内部广播，但又不知道本网络网络号，使用受限广播地址。受限广播地址路由器不会转发广播，所以是限制在本网段。
*/

/*
1、调用setsockopt对socket使能广播功能
2、向255.255.255.255:port地址进行广播，局域网中的主机接收端口号也为port
*/

int main()
{
	int sockfd; //服务端的端口描述符
	struct sockaddr_in server_addr; //服务端的IPV4地址结构体
	struct sockaddr_in broadcast_addr; //广播地址
	const int on = 1;
	char buf[256] = "Hello, I am server";
	
	//1、创建udp的socket
	if((sockfd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
		printf("create socket error\r\n");
		exit(0);
	}
	// bzero(&server_addr, sizeof(struct sockaddr_in));
	// server_addr.sin_family = AF_INET;
	// server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// server_addr.sin_port = htons(9000);
	// bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	//2、设置广播使能
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	
	//3、构造广播地址
	bzero(&broadcast_addr, sizeof(struct sockaddr_in));
	broadcast_addr.sin_family = AF_INET;
	// broadcast_addr.sin_addr.s_addr = inet_addr("172.21.255.255");
	broadcast_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
	broadcast_addr.sin_port = htons(8000);

	printf("udpserver running...\r\n");
	
	//4、发送广播
	while(1)
	{
		sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr));
		sleep(1);
	}
	
	//5、关闭连接
	close(sockfd);

	printf("end");
	return 0;
}
