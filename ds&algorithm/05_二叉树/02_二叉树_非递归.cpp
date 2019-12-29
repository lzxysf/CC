#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack>

using namespace std;

typedef int DataType;

typedef struct Tree{
  DataType data;//数据
  struct Tree* left;//左子节点
  struct Tree* right;//右子节点
}Tree;

//先序遍历
//入栈，先入右节点，再入左节点
//出栈，对出栈的元素，入其右节点，入其左节点
void pre_travel(Tree* tree)
{
  if(tree == NULL)
  {
    return;
  }
  //首先打印根节点
  printf("%d ", tree->data);
  //利用栈的后进后出的特点，用栈进行二叉树节点的存取
  stack<Tree*> s;
  //先将树的右节点放入栈中
  if(tree->right != NULL)
  {
    s.push(tree->right);
  }
  //再将数的左节点放入栈中
  if(tree->left != NULL)
  {
    s.push(tree->left);
  }
  //循环结束条件:栈为空了
  while(!s.empty())
  {
    //在栈中取出一个节点并打印
    Tree* temp = s.top();
    printf("%d ", temp->data);
    s.pop();
    //将这个节点的右子树和左子树先后放入栈中
    if(temp->right != NULL)
    {
      s.push(temp->right);
    }
    if(temp->left != NULL)
    {
      s.push(temp->left);
    }
  }
}

//查找最左端的节点
Tree* find_leftest(Tree* root, stack<Tree*> &s)
{
  while(root->left != NULL)
  {
    //将最左一列的节点依次入栈（不包括返回的最左节点）
    s.push(root);
    root = root->left;
  }
  //返回最左端的节点
  return root;
}

//中序遍历
void in_travel(Tree* tree)
{
  stack<Tree*> s;

  Tree* left = find_leftest(tree, s);
  while(left != NULL)
  {
    printf("%d ", left->data);
    if(left->right == NULL)
    {
      if(!s.empty())
      {
        left = s.top();
        s.pop();
      }
      else
      {
        left = NULL;
      }
    }
    else
    {
      left = find_leftest(left->right, s);
    }
  }
}


int main()
{
  //构造一个树
  /*
          1
        /   \
       2     3
     /   \  /  \
    4    5 6    7
  */
  Tree leaf1, leaf2, leaf3, leaf4, leaf5, leaf6, leaf7;
  memset(&leaf1, 0, sizeof(Tree));
  memset(&leaf2, 0, sizeof(Tree));
  memset(&leaf3, 0, sizeof(Tree));
  memset(&leaf4, 0, sizeof(Tree));
  memset(&leaf5, 0, sizeof(Tree));
  memset(&leaf6, 0, sizeof(Tree));
  memset(&leaf7, 0, sizeof(Tree));
  leaf1.data = 1;
  leaf2.data = 2;
  leaf3.data = 3;
  leaf4.data = 4;
  leaf5.data = 5;
  leaf6.data = 6;
  leaf7.data = 7;
  Tree* tree;
  tree = &leaf1;
  leaf1.left = &leaf2;
  leaf1.right = &leaf3;
  leaf2.left = &leaf4;
  leaf2.right = &leaf5;
  leaf3.left = &leaf6;
  leaf3.right = &leaf7;

  pre_travel(tree);
  printf("\r\n");
  in_travel(tree);
  printf("\r\n");
  return 0;
}
