#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//以顺序表形式存放的队列
//队头--------------队尾
//整个队列内存空间循环利用

#define MaxSize 10
typedef int DataType;

typedef struct Queue{
  DataType* data;
  int head;//队头
  int tail;//队尾
}Queue;

void queue_init(Queue* queue)
{
  if(queue == NULL)
  {
    return;
  }
  queue->data = (DataType*)malloc(sizeof(DataType)*MaxSize);
  queue->head = 0;
  queue->tail = 0;
}

void queue_destroy(Queue* queue)
{
  if(queue == NULL)
  {
    return;
  }
  if(queue->data != NULL)
  {
    free(queue->data);
  }
  queue->head = 0;
  queue->tail = 0;
}

//队尾插入元素
int push_back(Queue* queue, DataType item)
{
  int retval = -1;

  if(queue == NULL)
  {
    return retval;
  }
  //队列满了则返回-1
  if((queue->tail + 1) % MaxSize == queue->head)
  {
    return retval;
  }
  queue->data[queue->tail] = item;
  queue->tail = (queue->tail + 1) % MaxSize;
  retval = 0;
  return retval;
}

//队头取出元素
int pop_front(Queue* queue, DataType* item)
{
  int retval = -1;
  if(queue == NULL)
  {
    return retval;
  }
  //队列空了则返回-1
  if(queue->head == queue->tail)
  {
    return retval;
  }
  *item = queue->data[queue->head];
  queue->head = (queue->head + 1) % MaxSize;
  retval = 0;
  return retval;
}

int i = 100;
void* my_run(void* arg)
{
  Queue* queue = (Queue*)arg;
  while(1)
  {
    int ret = push_back(queue, i++);
    usleep(1000000);
  }
}

int main()
{
  Queue queue;
  queue_init(&queue);

  pthread_t tid;
  pthread_create(&tid, NULL, my_run, &queue);

  while(1)
  {
    int item;
    if(pop_front(&queue, &item)==0)
    {
      printf("取出的队头元素为:%d\r\n", item);
    }
    usleep(500000);
  }

  return 0;
}
