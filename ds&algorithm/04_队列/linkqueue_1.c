#include <stdio.h>
#include <stdlib.h>

//以栈形式存放的队列
//队尾----------------队头

typedef int DataType;

typedef struct Node{
  DataType data;
  struct Node* next;
}Node;

typedef struct Queue{
  Node* head;
  Node* tail;
}Queue;

void queue_init(Queue* queue)
{
  queue->head = NULL;
  queue->tail = NULL;
}

void queue_destroy(Queue* queue)
{
  queue_init(queue);
}

//队尾插入元素
void push_back(Queue* queue, DataType data)
{
  Node* item = (Node*)malloc(sizeof(Node));
  item->data = data;

  //如果插入的是第一个元素
  if(queue->head == NULL)
  {
    queue->head = item;
    queue->tail = item;
    item->next = NULL;
    return;
  }
  
  //如果插入的不是第一个元素
  item->next = queue->tail;
  queue->tail = item;
}

//队头取出元素
DataType pop_front(Queue* queue)
{
  /*队头节点与元素*/
  Node* node = queue->head;
  DataType data = node->data;

  //如果队列只剩一个节点，清空队列
  if(queue->head == queue->tail)
  {
    queue->head = NULL;
    queue->head = NULL;
    return data;
  }

  //如果队列不是只剩一个节点
  /*定位到队头节点的上一个节点*/
  Node* temp = queue->tail;
  while(temp->next != queue->head)
  {
    temp = temp->next;
  }
  temp->next = NULL;
  queue->head = temp;

  return data;
}

int is_empty(Queue* queue)
{
  if(queue->head == NULL)
  {
    return 1;
  }
  return 0;
}

int main()
{
  Queue queue;
  queue_init(&queue);
  push_back(&queue, 100);
  push_back(&queue, 200);
  push_back(&queue, 300);
  push_back(&queue, 400);
  while(!is_empty(&queue))
  {
    printf("队列中取出元素%d\n", pop_front(&queue));
  }
  return 0;
}
