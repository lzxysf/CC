#include <stdio.h>
#include <stdlib.h>

typedef int DataType;
typedef struct Node {
  DataType data;
  struct Node* next;
} Node;
typedef struct RecyLinklist{
  Node* header;
  Node* tail;
} RecyLinklist;

void init_list(RecyLinklist* list)
{
  list->header = NULL;
  list->tail = NULL;
}

void destroy_list(RecyLinklist* list)
{
  init_list(list);
}

void push_back(RecyLinklist* list, DataType data)
{
  if(list == NULL)
  {
    return;
  }

  Node* item = (Node*)malloc(sizeof(Node));
  item->data = data;

  //如果是添加第一个数据
  if(list->header == NULL)
  {
    list->header = item;
    list->tail = item;
    item->next = item;
    return;
  }

  list->tail->next = item;
  item->next = list->header;
  list->tail = item;
}

int get_length(RecyLinklist* list)
{
    int length = 0;
    Node* node = list->header;
    while(node!=list->tail)
    {
        length++;
        node = node->next;
    }
    length++;
    return length;
}

void print(RecyLinklist* list)
{
  //list为空，返回
  if(list == NULL)
  {
    return;
  }
  //如果list中没有元素，返回
  if(list->header == NULL) return;

  Node* node = list->header;
  printf("%d ", node->data);
  node = node->next;
  while(node->next != list->header)
  {
    printf("%d ", node->data);
    node = node->next;
  }
  printf("%d", node->data);
  printf("\r\n");
}

int main()
{
  RecyLinklist list;
  init_list(&list);
  push_back(&list, 100);
  push_back(&list, 200);
  push_back(&list, 300);
  push_back(&list, 400);
  print(&list);
  int length = get_length(&list);
  printf("链表的长度%d\r\n", length);
  return 0;
}
