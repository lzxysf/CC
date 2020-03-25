#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

int main()
{
    void* context = zmq_ctx_new();
    void* receiver = zmq_socket(context, ZMQ_PULL);
    zmq_connect(receiver, "tcp://localhost:5557");

    while(1)
    {
        char buffer[100]={0};

        zmq_recv(receiver, buffer, sizeof(buffer), 0);
 
        printf("pull data is %s\r\n", buffer);

        sleep(1);
    }

    return 0;
}