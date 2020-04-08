#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <zmq.h>

int main()
{
    void* context = zmq_ctx_new();
    void* response = zmq_socket(context, ZMQ_PUB);
    int rc = zmq_bind(response, "tcp://*:5556");
    if(rc!=0)
    {
        perror("zmq_bind error:");
        exit(0);
    }

    while(1)
    {
        char buffer[100]="10002 hello,udn,utype";

        zmq_send(response, buffer, strlen(buffer), 0);

        sleep(1);
    }

    return 0;
}
