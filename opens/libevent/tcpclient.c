#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main()
{
	int sockfd;//客户端的端口描述符
	struct sockaddr_in server_addr;
	char buffer[256];
	
	if((sockfd=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		printf("create socket error\r\n");
		exit(0);
	}
	
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9995);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(sockfd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr)) == -1)
	{
		printf("connect error\r\n");
		exit(1);
	}
	printf("连接到服务器成功\r\n");
	
	while(read(sockfd, buffer, sizeof(buffer)) > 0)
	{
		printf("%s\r\n", buffer);
	}

	close(sockfd);
	printf("done\r\n");
	return 0;
}
