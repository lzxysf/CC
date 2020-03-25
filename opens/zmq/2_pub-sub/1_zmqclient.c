#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

int main()
{
    void* context = zmq_ctx_new();
    void* subscribe = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscribe, "tcp://localhost:5556");

    char filter1[] = "10001 ";
    char filter2[] = "10002 ";    
    zmq_setsockopt(subscribe, ZMQ_SUBSCRIBE, filter1, strlen(filter1));
    zmq_setsockopt(subscribe, ZMQ_SUBSCRIBE, filter2, strlen(filter2));

    while(1)
    {
        char buffer[100]={0};

        zmq_recv(subscribe, buffer, sizeof(buffer), 0);
        
        printf("subscribe data is %s\r\n", buffer);

        sleep(1);
    }

    return 0;
}
