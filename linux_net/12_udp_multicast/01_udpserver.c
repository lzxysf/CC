#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntop
#include <net/if.h>

#define MAXLINE 80
#define GROUP "239.0.0.2"

/*
多播(组播)(multicast)
239.0.0.0～239.255.255.255为本地管理组播地址，仅在特定的本地范围内有效

组播服务器创建步骤:
1、创建组地址，并使用setsockopt设置组播接口
2、创建客户端地址，ip为组播地址，端口任意
3、发送数据到客户端地址，此组中客户端会收到数据
*/

int main()
{
	int sockfd; //服务端的端口描述符
	struct sockaddr_in server_addr; //服务端的IPV4地址结构体
	struct sockaddr_in multicast_addr; //组播地址（客户端地址）
	const int on = 1;
	char buf[256] = "Hello, I am server";
	struct ip_mreqn group;
	
	if((sockfd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
		printf("create socket error\r\n");
		exit(0);
	}

	//1、创建组地址，并使用setsockopt设置组播
	inet_pton(AF_INET, GROUP, &group.imr_multiaddr);
	inet_pton(AF_INET, "0.0.0.0", &group.imr_address);
	group.imr_ifindex = if_nametoindex("eth0");
	setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &group, sizeof(group));

	//2、创建客户端地址，ip为组播地址，端口任意
	bzero(&multicast_addr, sizeof(struct sockaddr_in));
	multicast_addr.sin_family = AF_INET;
	inet_pton(AF_INET, GROUP, &multicast_addr.sin_addr.s_addr);
	multicast_addr.sin_port = htons(6000);

	//3、发送数据到客户端地址，此组中客户端会收到数据
	while(1)
	{
		sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&multicast_addr, sizeof(multicast_addr));
		sleep(1);
	}

	close(sockfd);
	printf("end");
	return 0;
}
