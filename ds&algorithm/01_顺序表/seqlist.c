#include <stdio.h>
#include <stdlib.h>

#define MaxSize 100
typedef int DataType;

typedef struct{
  DataType* data;
  int length;
  int size;
}seqlist;

//顺序表一般表现为数组

//初始化顺序表
int Init_List(seqlist* L)
{
  L->data = (DataType*)malloc(sizeof(DataType)*MaxSize);
  if(L->data == NULL)
  {
    return -1;
  }
  L->length = 0;
  L->size = MaxSize;
  return 0;
}

int Destroy_List(seqlist* L)
{
  if(L && L->data)
  {
    free(L->data);
  }
  else
  {
    return -1;
  }
  L->length = 0;
  L->size = 0;
  return 0;
}

//判断顺序表是否为空
bool is_empty(seqlist L)
{
  return L.length == 0;
}

//获取顺序表的长度
int get_length(seqlist L)
{
  return L.length;
}

//获取顺序表的第i个位置的元素值
int get_item(seqlist L, int i, DataType* item)
{
  if(i > 0 && i < L.length)
  {
    *item = L.data[i];
    return 0;
  }
  else
  {
    return -1;
  }
}

//顺序表末尾添加元素
int add_item(seqlist* L, DataType item)
{
  L->length++;
  if(L->length > L->size)
  {
    L->size = L->size * 2;
    realloc(L->data, sizeof(DataType)*L->size);
  }

  L->data[L->length-1] = item;
}

//顺序表中任意位置插入元素
int insert_item(seqlist* L, int pos, DataType item)
{
  if(pos < 0 || pos > L->length)
  {
    return -1;
  }

  //将最后一个位置的元素向后移动一个位置，...，将pos位置的元素向后移动一个位置，
  //然后将pos位置置为item
  for(int i = L->length - 1; i >= pos; i--)
  {
    L->data[i+1] = L->data[i];
  }
  L->data[pos] = item;
  L->length++;//不要忘了插入后顺序表长度要加1

  return 0;
}

//顺序表任意位置删除元素,删除的元素用item返回
int remove_item(seqlist* L, int pos, DataType* item)
{
  if(pos < 0 || pos >= L->length)
  {
    return -1;
  }

  if(item != NULL)
  {
    *item = L->data[pos];
  }

  //将pos+1位置的元素向前移动一个位置，...，最后将length-1位置的元素（即最后一个元素）向前移动一个位置
  for(int i = pos + 1; i <= L->length-1; i++)
  {
    L->data[i-1] = L->data[i];
  }
  L->length--;

  return 0;
}

//顺序表打印
int print_list(seqlist L)
{
  for(int i = 0; i < L.length; i++)
  {
    printf("%d ", L.data[i]);
  }
  printf("\n");
}

int main()
{
  seqlist L;
  Init_List(&L);
  // Destroy_List(&L);
  bool ret = is_empty(L);
  printf("是否为空:%d\n", ret);

  add_item(&L, 10);
  add_item(&L, 20);
  add_item(&L, 15);
  ret = is_empty(L);
  printf("是否为空:%d\n", ret);
  
  int retval = get_length(L);
  printf("list的长度为:%d\n", retval);

  print_list(L);

  //在位置2处插入66
  insert_item(&L, 2, 66);
  print_list(L);

  DataType item;
  remove_item(&L, 2, &item);
  print_list(L);
  printf("被删除的元素为:%d\n", item);

  return 1;
}
