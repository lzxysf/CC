#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//01.以顺序表形式存放的栈

#define MaxSize 10
typedef int DataType;

typedef struct Stack{
  DataType* top;//栈顶
  DataType* bottom;//栈底
  DataType *data;//以顺序表形式存放栈的数据
}Stack;

int init_stack(Stack* stack)
{
  stack->data = (DataType*)malloc(sizeof(DataType)*MaxSize);
  memset(stack->data, 0, sizeof(DataType)*MaxSize);
  stack->top = NULL;
  stack->bottom = NULL;
}

int is_empty(Stack* stack)
{
    if(stack->top == NULL)
    {
        return 1;
    }
    return 0;
}

int is_full(Stack* stack)
{
    if (stack->top == stack->bottom + MaxSize - 1)
    {
        return 1;
    }
    return 0;
}

void push(Stack* stack, DataType item)
{
  //如果栈满了,返回
  if(stack->top == stack->bottom + MaxSize - 1)
  {
      return;
  }
  //如果是第一个入栈
  if(stack->top == NULL)
  {
    stack->top = stack->data;
    stack->bottom = stack->data;
    *stack->top = item;
    return;
  }
  //如果不是第一个入栈
  stack->top++;
  *stack->top = item;
}

DataType pop(Stack* stack)
{
    DataType temp = *stack->top;
    stack->top--;
    return temp;
}

void print(Stack* stack)
{
    while(stack->bottom <= stack->top)
    {
        printf("%d ", *stack->bottom);
        stack->bottom++;
    }
    printf("\r\n");
}

int main()
{
  Stack stack;
  init_stack(&stack);
  printf("栈是否为空:%d\n", is_empty(&stack));
  push(&stack, 100);
  push(&stack, 200);
  push(&stack, 300);
  push(&stack, 400);
  push(&stack, 666);
  push(&stack, 777);
  printf("栈是否为空:%d\n", is_empty(&stack));
  print(&stack);

  DataType data = pop(&stack);
  printf("出栈元素:%d\r\n", data);
  data = pop(&stack);
  printf("出栈元素:%d\r\n", data);
  return 0;
}
