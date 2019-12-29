#include <stdio.h>
#include <stdlib.h>

//以顺序表形式存放的队列
//队尾----------------队头

#define MaxSize 100
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
  queue->head = -1;
  queue->tail = -1;
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
  queue->head = -1;
  queue->tail = -1;
}

//队尾插入元素
void push_back(Queue* queue, DataType item)
{
  //如果队列已经满了
  if(queue->top >= MaxSize-1)
  {
    return;
  }
  //如果插入的是第一个元素
  if(queue->head == -1 && queue->tail == -1)
  {
    queue->head = 0;
    queue->tail = 0;
    queue->data[queue->head] = item;
    return;
  }

  //如果插入的不是第一个元素
  //length为队列长度
  int length = queue->head + 1;
  while(length!=0)
  {
    queue->data[length] = queue->data[length-1];
    length--;
  }
  queue->data[0] = item;
  queue->head = queue->head + 1;
}

//队头取出元素
DataType pop_front(Queue* queue)
{
  DataType head = queue->data[queue->head];
  queue->head--;
  return head;
}

int get_length(Queue* queue)
{
  return queue->head+1;
}

int main()
{
  Queue queue;
  queue_init(&queue);
  push_back(&queue, 100);
  push_back(&queue, 200);
  push_back(&queue, 300);
  printf("队列长度%d\n", get_length(&queue));
  printf("队头取出的元素为%d\n", pop_front(&queue));
  printf("队头取出的元素为%d\n", pop_front(&queue));
  return 0;
}
