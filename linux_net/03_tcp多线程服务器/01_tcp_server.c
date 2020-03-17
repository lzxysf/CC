/*
此处多线程服务器指的是将服务端与客户端的交互逻辑放到一个新的线程中执行
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntop
#include <pthread.h>

#define MAXLINE 80 //一次读取客户端的数据长度

struct client_info{
	int newfd;//客户端端口描述符
	struct sockaddr_in client_addr;//客户端的IPV4地址结构体
};

void* dowork(void *arg)
{
	struct client_info* clientinfo;
	int newfd;//客户端的端口描述符
	struct sockaddr_in client_addr;//客户端的IPV4地址结构体
	char client_addr_str[INET_ADDRSTRLEN];//客户端地址字符串
	int port;//客户端端口号
	char buffer[MAXLINE];//读取数据缓冲区

	pthread_detach(pthread_self());
	
	clientinfo = (struct client_info*)arg;
	newfd = clientinfo->newfd;
	client_addr = clientinfo->client_addr;
	
	inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_str, sizeof(client_addr_str));
	port = ntohs(client_addr.sin_port); 
	printf("a client connected, ip is %s, port is %d\r\n", client_addr_str, port);

	//read是阻塞函数，当客户端关闭，此处read会返回0
	//tcp收到fin，socket产生eof并发送SIGURG信号，睡眠的read被唤醒，发现eof，返回已读取字符数(0)
	while(read(newfd, buffer, sizeof(buffer)) > 0)
	{
		printf("读取到的数据为:%s\r\n", buffer);
	}
	
	printf("读取结束。。。\r\n");
	close(newfd);	

	return NULL;
}

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
		pthread_t tid;
		int newfd;//客户端的端口描述符
		struct sockaddr_in client_addr;//客户端的IPV4地址结构体
		int size = sizeof(struct sockaddr);//客户端地址长度	
	
		//5、等待客户端连接
		newfd = accept(sockfd, (struct sockaddr*)&client_addr, &size);
		
		struct client_info info;
		info.newfd = newfd;
		info.client_addr = client_addr;	
		pthread_create(&tid, NULL, dowork, (void*)&info);		
	}
	
	//6、关闭连接
	close(sockfd);
		
	printf("end");
	return 0;
}
