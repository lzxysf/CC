#include <iostream>
#include <stack>
using namespace std;

typedef struct ListNode{
  int val;
  struct ListNode* next;
}ListNode;

ListNode* listnodetwoswap(ListNode* head)
{
  if(head==NULL || head->next==NULL) return head;
  
  stack<ListNode*> mys;
  ListNode* lnode = head;
  ListNode* node = new ListNode();
  ListNode* header = node;
  while(lnode&&lnode->next)
  {
    mys.push(lnode);
    mys.push(lnode->next);
    lnode = lnode->next->next;
    
    ListNode* temp = mys.top();
    mys.pop();
    node->next = temp;
    node = node->next;
    temp = mys.top();
    mys.pop();
    node->next = temp;
    node = node->next;
  }

  if(lnode)
  {
    node->next = lnode;
  }
  else
  {
    node->next = NULL;
  }
  return header->next;
}

int main()
{
  ListNode* node = new ListNode();
  ListNode* head = node;
  for(int i = 0; i < 100; i++)
  {
    ListNode* item = new ListNode();
    item->val = i;
    item->next = NULL;
    node->next = item;
    node = item;
  }
  head = head->next;

  ListNode* ret = listnodetwoswap(head);

  while(ret)
  {
    cout << ret->val << " ";
    ret = ret->next;
  }

  return 0;
}
