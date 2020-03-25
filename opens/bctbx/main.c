#include <stdio.h>
#include <linux/socket.h>
#include <sys/ioctl.h>
#include <bctoolbox/port.h>
#include <bctoolbox/logging.h>
#include <bctoolbox/vconnect.h>

#if 1
typedef struct{
  int data;
  struct Node* next;
}Node;

int main()
{
  uint64_t t = bctbx_get_cur_time_ms();
  printf("time:%ld\r\n", t);

  unsigned int ret = bctbx_random();
  printf("随机数:%u\r\n", ret);

  Node* node = bctbx_malloc0(sizeof(Node));
  node->data = 100;
  node->next = NULL;
  printf("header data is %d\r\n", node->data);
  bctbx_free(node);

  return 0;
}
#endif

#if 0
int main()
{
  int count = 0;

  bctbx_socket_t rpc_sock = bctbx_socket(AF_INET, SOCK_DGRAM, 0);
  char* rpc_ip = "tcp://127.0.0.1:60001";
  
  struct addrinfo hints, *res;
  hints.ai_family = PF_UNSPEC;
  hints.ai_flags = AI_NUMERICHOST;
  hints.ai_socktype = SOCK_DGRAM;
  getaddrinfo("127.0.0.1", "60001",&hints,&res);
  if(bctbx_connect(rpc_sock, res->ai_addr, res->ai_addrlen) < 0)
  {
    printf("connect error!");
  }
  else
  {
    printf("connect success!");
  }

  ioctl(rpc_sock, FIONBIO, 1); 

  char* sendstr = "exit";
  int size = strlen(sendstr);
  bctbx_send(rpc_sock, sendstr, size, 0);

  freeaddrinfo(res);

  return 0;
}
#endif

