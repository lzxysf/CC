#include <stdio.h>
#include <stdlib.h>

typedef int DataType;

typedef struct Node{
  DataType data;
  struct Node* next;
  struct Node* prev; 
}Node;

typedef struct {
  Node* header;
}DouLinklist;

void init_list(DouLinklist* list)
{
  list->header = NULL;
}

void destroy_list(DouLinklist* list)
{
  init_list(list);
}

void push_back(DouLinklist* list, DataType data)
{
  if(list == NULL) return;

  Node* item = (Node*)malloc(sizeof(Node));
  item->data = data;
  item->next = NULL;

  //如果链表为空
  if(list->header == NULL)
  {
    list->header = item;
    item->prev = NULL;
    return;//不要忘了return
  }
  //如果链表不为空
  Node* node = list->header;
  while(node->next!=NULL)
  {
    node = node->next;
  }
  node->next = item;
  item->prev = node;
}

void insert(DouLinklist* list, int pos, DataType data)
{
  if(list == NULL) return;

  Node* item = (Node*)malloc(sizeof(Node));
  item->data = data;

  //如果是在第一个位置插入
  if(pos == 0)
  {
    item->next = list->header;
    item->prev = NULL;
    list->header = item;
    return;
  }

  Node* node = list->header;
  for(int i = 0; i < pos - 1; i++)
  {
    node = node->next;
  }

  //如果是最后一个位置插入
  if(node->next == NULL)
  {
    item->prev = node;
    item->next = NULL;
    node->next = item;
    return;
  }

  //如果是中间位置插入
  item->next = node->next;
  item->prev = node;
  node->next->prev = item;
  node->next = item;
}

int remove(DouLinklist* list, int pos, DataType* item)
{
  int length = 0;
  if (list == NULL || pos < 0)
  {
    return -1;
  }

  Node* node = list->header;
  while(node)
  {
    node = node->next;
    length++;
  }
  if(pos >= length)
  {
    return -1;
  }

  node = list->header;
  //如果要删除的是第一个元素
  if(pos == 0)
  {
    node->next->prev = NULL;
    list->header = node->next;
  }

  for(int i = 0; i < pos-1; i++)
  {
    node = node->next;
  }
  //如果要删除的是最后一个元素
  if(node->next->next == NULL)
  {
    node->next = NULL;
    return 0;
  }
  //如果要删除的是中间的元素
  node->next = node->next->next;
  node->next->prev = node;
}

int get_item(DouLinklist* list, int pos, DataType* item)
{
  int length = 0;
  //如果list为空或者pos小于0，参数不正确，返回
  if(list==NULL || pos < 0)
  {
    return -1;
  }
  //获得list的长度
  Node* node = list->header;
  while(node)
  {
    length++;
    node = node->next;
  }
  //如果pos大于list的长度，参数不正确，返回
  if(pos >= length)
  {
    return -1;
  }

  //重置node为list的头指针
  node = list->header;
  for(int i = 0; i < pos; i++)
  {
    node = node->next;
  }
  *item = node->data;
  return 0;
}

void print(DouLinklist* list)
{
  if(list == NULL) return;

  Node* node = list->header;
  while(node!=NULL)
  {
    printf("%d ", node->data);
    node = node->next;
  }
  printf("\r\n");
}

int main()
{
  DouLinklist list;
  init_list(&list);
  push_back(&list, 100);
  push_back(&list, 200);
  push_back(&list, 500);
  print(&list);
  insert(&list, 0, 99);
  insert(&list, 4, 900);
  insert(&list, 3, 400);
  print(&list);

  DataType item;
  int pos = 5;
  int ret = get_item(&list, pos, &item);
  if(ret==0)
  {
    printf("位置%d处的元素为%d\r\n", pos, item);
  }
  else
  {
    printf("获得元素失败!");
  }

  remove(&list, 0, NULL);
  print(&list);

  return 0;
}
