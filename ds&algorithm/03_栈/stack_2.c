#include <stdio.h>
#include <stdlib.h>

//02.以链表形式存放的栈

typedef int DataType;

typedef struct Node{
  DataType data;
  struct Node* next;
}Node;

typedef struct Stack{
  Node* top;//栈顶
  Node* bottom;//栈底
}Stack;

void init_stack(Stack* stack)
{
  stack->top = NULL;
  stack->bottom = NULL;
}

void push(Stack* stack, DataType data)
{
  Node* item = (Node*)malloc(sizeof(Node));
  item->data = data;
  item->next = NULL;
  //如果是第一个元素
  if(stack->top == NULL)
  {
    stack->top = item;
    stack->bottom = item;
    return;
  }
  //如果不是第一个元素
  stack->top->next = item;
  stack->top = item;
}

void pop(Stack* stack)
{
    //如果只有一个元素
    if(stack->top == stack->bottom)
    {
        stack->top = NULL;
        stack->bottom = NULL;
    }
    //如果有不止一个元素
    Node* node = stack->bottom;
    while(node->next != stack->top)
    {
        node = node->next;
    }

    stack->top = node;
}

DataType top(Stack* stack)
{
    return stack->top->data;
}

int is_empty(Stack* stack)
{
    if(stack->top == NULL)
    {
        return 1;
    }
    return 0;
}

int main()
{
    Stack stack;
    init_stack(&stack);
    printf("是否为空:%d\n", is_empty(&stack));
    push(&stack, 100);
    push(&stack, 200);
    push(&stack, 300);
    push(&stack, 400);
    printf("是否为空:%d\n", is_empty(&stack));
    printf("栈顶元素%d\n", top(&stack));

    pop(&stack);
    printf("栈顶元素%d\n", top(&stack));
    pop(&stack);
    printf("栈顶元素%d\n", top(&stack));
    pop(&stack);
    printf("栈顶元素%d\n", top(&stack));

    return 0;
}
