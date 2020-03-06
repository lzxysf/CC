/*
此处多进程服务器指的是将服务端与客户端的交互逻辑放到一个新的进程中执行
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntop
#include <sys/wait.h>
#include <signal.h>

#define MAXLINE 80 //一次读取客户端的数据长度

void collect_child_handler(int signo)
{
	pid_t pid;
	while((pid=waitpid(0, NULL, WNOHANG))>0)
	{
		printf("进程%d回收成功\r\n", pid);
	}
	printf("回收完成\r\n");
}

int main()
{
	struct sigaction act, old_act;
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
		
	//设置SIGCHLD信号捕获函数
	act.sa_handler = collect_child_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGCHLD, &act, &old_act);

	while(1)
	{
		pid_t pid;
		int newfd;//客户端的端口描述符
		struct sockaddr_in client_addr;//客户端的IPV4地址结构体
		int size = sizeof(struct sockaddr);//客户端地址长度	
		char client_addr_str[INET_ADDRSTRLEN];//客户端地址字符串
		int port;//客户端端口号
		char buffer[MAXLINE];//读取数据缓冲区
	
		//5、等待客户端连接
		newfd = accept(sockfd, (struct sockaddr*)&client_addr, &size);

		pid = fork();
		if(pid < 0)
		{
			perror("fork error\r\n");
			exit(0);
		}
		else if(pid > 0)
		{
			close(newfd);
			while(1);
		}
		else
		{
			sigaction(SIGCHLD, &old_act, NULL);
			close(sockfd);
		
			inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_str, sizeof(client_addr_str));
			port = ntohs(client_addr.sin_port); 
			printf("a client connected, ip is %s, port is %d\r\n", client_addr_str, port);

			while(read(newfd, buffer, sizeof(buffer)) > 0)
			{
				printf("读取到的数据为:%s\r\n", buffer);
			}
			printf("读取结束。。。\r\n");
			close(newfd);
			return 0;
		}	
	}
	
	//6、关闭连接
	close(sockfd);
		
	printf("end");
	return 0;
}
