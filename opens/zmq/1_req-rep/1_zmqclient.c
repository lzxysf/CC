#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

int main()
{
    void* context = zmq_ctx_new();
    void* request = zmq_socket(context, ZMQ_REQ);
    zmq_connect(request, "tcp://localhost:5555");

    while(1)
    {
        char buffer[100];
        char buf[100] = "helloWorld!";

        zmq_send(request, buf, strlen(buf), 0);
        zmq_recv(request, buffer, sizeof(buffer), 0);
        
        printf("reply data is %s\r\n", buffer);

        sleep(1);
    }

    return 0;
}