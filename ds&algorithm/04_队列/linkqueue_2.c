#include <stdio.h>
#include <stdlib.h>

//以栈形式存放的队列2
//队头---------------队尾

typedef char* DataType;

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
  if(queue->head == NULL && queue->tail == NULL)
  {
    queue->head = item;
    queue->tail = item;
    return;
  }
  //如果插入的不是第一个元素
  queue->tail->next = item;
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
  queue->head = queue->head->next;

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
  push_back(&queue, "abc");
  push_back(&queue, "hello");
  push_back(&queue, "zhaoyejia");
  push_back(&queue, "lixinran");
  while(!is_empty(&queue))
  {
    printf("队列中取出元素%s\n", pop_front(&queue));
  }
  return 0;
}
