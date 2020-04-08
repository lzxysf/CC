#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntop
#include <net/if.h>

#define MAXLINE 80
#define GROUP "239.0.0.2"

/*
在客户端中接收服务端发送组播消息
1、绑定本地地址，ip为本地任意ip，端口号为组播端口号
2、创建组播地址，使用setsockopt加入组播
*/

int main()
{
	int sockfd;//客户端的端口描述符
	struct sockaddr_in client_addr;//客户端的IPV4地址结构体
	struct ip_mreqn group;

	if((sockfd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
		printf("create socket error\r\n");
		exit(0);
	}
	
	//1、绑定本地地址，ip为本地任意ip，端口号为组播端口号
	bzero(&client_addr, sizeof(struct sockaddr_in));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(6000);
	client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockfd, (struct sockaddr*)&client_addr, sizeof(struct sockaddr));

	//2、创建组播地址，使用setsockopt加入组播
	inet_pton(AF_INET, GROUP, &group.imr_multiaddr);
	inet_pton(AF_INET, "0.0.0.0", &group.imr_address);
	group.imr_ifindex = if_nametoindex("eth0");
	setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group));
	
	while(1)
	{
		char buffer[MAXLINE]={0};
		int len = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, 0);
		printf("recv data is %s\r\n", buffer);
	}
	
	close(sockfd);	
	printf("end");
	return 0;
}
