#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <zmq.h>

int main()
{
    void* context = zmq_ctx_new();
    void* response = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(response, "tcp://*:5555");
    if(rc!=0)
    {
        perror("zmq_bind error:");
        exit(0);
    }

    while(1)
    {
        char buffer[100]={0};
        zmq_recv(response, buffer, sizeof(buffer), 0);
        printf("receive data is %s\r\n", buffer);
        for(int i = 0; i < strlen(buffer); i++)
        {
            buffer[i] = toupper(buffer[i]);
        }
        zmq_send(response, buffer, strlen(buffer), 0);
    }

    return 0;
}
