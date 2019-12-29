#include <stdio.h>
#include <stdlib.h>

typedef int DataType;

typedef struct Node{
  DataType data;
  struct Node* next;
}Node;

typedef struct LinkList{
  Node* header;
} LinkList;

void list_init(LinkList* list)
{
  list->header = NULL;
}

void list_destroy(LinkList* list)
{
  list->header = NULL;
}

int get_length(LinkList* list)
{
  int count = 0;
  Node* node = list->header;
  while(node!=NULL)
  {
    node = node->next;
    count++;
  }
  return count;
}

void push_back(LinkList* list, DataType data)
{
  Node* item = (Node*)malloc(sizeof(Node));
  item->data = data;
  item->next = NULL;

  Node* node = list->header;

  //如果是第一次插入数据
  if(node == NULL)
  {
    list->header = item;
    return;
  }
  
  //如果不是第一次插入数据
  //找到最后一个元素所在位置，在其后插入元素
  while(node->next != NULL)
  {
    node = node->next;
  }

  node->next = item;
}

void insert(LinkList* list, int pos, DataType data)
{
  int length = get_length(list);
  if(pos < 0 || pos > length)
  {
    return;
  }

  Node* item = (Node*)malloc(sizeof(Node));
  item->data = data;

  if(pos == 0)
  {
    item->next = list->header;
    list->header = item;
    return;
  }

  Node* node = list->header;
  for(int i = 0; i < pos-1; i++)
  {
    node = node->next;
  }

  item->next = node->next;
  node->next = item;
}

void remove(LinkList* list, int pos)
{
  int length = get_length(list);
  if(pos < 0 || pos >= length)
  {
    return;
  }

  if(pos==0)
  {
    list->header = list->header->next;
    return;
  }

  Node* node = list->header;
  for(int i = 0; i < pos-1; i++)
  {
    node = node->next;
  }

  node->next = node->next->next;
}

//获得pos位置处的Node*节点
Node* get_item(LinkList* list, int pos)
{
  Node* node = list->header;
  while(pos--)
  {
    node = node->next;
  }
  return node;
}

void print(LinkList* list)
{
  Node* node = list->header;
  while(node != NULL)
  {
    printf("%d ", node->data);
    node = node->next;
  }
  printf("\r\n");
}

int main()
{
  LinkList list;
  list_init(&list);
  push_back(&list, 100);
  push_back(&list, 200);
  push_back(&list, 500);
  print(&list);
  int length = get_length(&list);
  printf("\n当前链表长度:%d\n",length);
  
  insert(&list,1,555);
  insert(&list,0,111);
  insert(&list,5,888);
  push_back(&list,999);
  print(&list);

  remove(&list,0);
  remove(&list,4);
  print(&list);

  Node* item = get_item(&list,2);
  printf("pos处节点的数值:%d\n", item->data);
  printf("pos下一处节点的数值:%d\n", item->next->data);
  return 0;
}
