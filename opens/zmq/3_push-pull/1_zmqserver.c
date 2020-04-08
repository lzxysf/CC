#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <zmq.h>

int main()
{
    char buf[] = "task";

    void* context = zmq_ctx_new();
    void* sender = zmq_socket(context, ZMQ_PUSH);
    int rc = zmq_bind(sender, "tcp://*:5557");
    if(rc!=0)
    {
        perror("zmq_bind error:");
        exit(0);
    }

    while(1)
    {
        static int count = 0;
        char buffer[100]={0};

        count++;
        sprintf(buffer, "%s%d", buf, count);
        zmq_send(sender, buffer, strlen(buffer), 0);

        usleep(400000); //0.4s
    }

    return 0;
}
