#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

typedef int DataType;

typedef struct Tree{
  DataType data;//数据
  struct Tree* left;//左子节点
  struct Tree* right;//右子节点
}Tree;


//先序遍历
void pre_travel(Tree* tree)
{
  //递归结束条件
  if(tree == NULL)
  {
    return;
  }

  //访问根节点
  printf("%d ", tree->data);

  //先序遍历左子树
  pre_travel(tree->left);

  //先序遍历右子树
  pre_travel(tree->right);
}

//中序遍历
void in_travel(Tree* tree)
{
  if(tree==NULL)
  {
    return;
  }
  in_travel(tree->left);
  printf("%d ", tree->data);
  in_travel(tree->right);
}

//后续遍历
void post_travel(Tree* tree)
{
  if(tree==NULL)
  {
    return;
  }
  post_travel(tree->left);
  post_travel(tree->right);
  printf("%d ", tree->data);
}


//求树的叶子节点的个数
void leaf_num(Tree* tree, int *count)
{
  //递归结束的条件，空树
  if(tree == NULL)
  {
      return;
  }
  //遍历左子树
  leaf_num(tree->left, count);
  //遍历右子树
  leaf_num(tree->right, count);
  //叶子节点,叶子节点的左子树和右子树都是空
  if(tree->left == NULL && tree->right == NULL)
  {
      (*count)++;
  }
}

//求树的高度
int tree_high(Tree* tree)
{
  if(tree == NULL)
  {
    return 0;
  }

  int left_high = tree_high(tree->left);
  int right_high = tree_high(tree->right);
  int high = left_high > right_high ? left_high : right_high;
  return high + 1;//返回左右子树中值最大的，再加上当前父节点，因为不会统计父节点
}

//二叉树拷贝
void copy_tree(Tree* tree, Tree* copy)
{
  if(tree == NULL)
  {
    return;
  }
  
  Tree* temp = (Tree*)malloc(sizeof(Tree));
  temp->data = tree->data;
  copy = temp;

  copy_tree(tree->left, copy->left);
  copy_tree(tree->right, copy->right);
}


//#号法创建一棵树-按照先序遍历的顺序创建
//如果要创建main中的二叉树，输入的顺序为1 2 4 # # 5 # # 3 6 # # 7 # # ,即如果一个节点它没有左右节点那么需要输入# #，如果没有左节点或右节点，输入#
Tree* create_tree()
{
  char input;
  cout << "input current node's value" << endl;
  cin >> input;
  if(input == '#')
  {
    return NULL;
  }
  Tree* root = (Tree*)malloc(sizeof(Tree));
  root->data = input - '0';
  root->left = create_tree();
  root->right = create_tree();
  return root;
}

void destroy_tree(Tree* tree)
{
  if(tree)
  {
    if(tree->left)
    {
      destroy_tree(tree->left);
    }
    if(tree->right)
    {
      destroy_tree(tree->right);
    }
    free(tree);
    tree = NULL;
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

/*
  Tree* tree = NULL;
  tree = create_tree();
  destroy_tree(tree);
*/
  pre_travel(tree);
  printf("\r\n");
  in_travel(tree);
  printf("\r\n");
  post_travel(tree);
  printf("\r\n");

  int num = 0;
  leaf_num(tree, &num);
  printf("叶子节点的个数：%d\r\n", num);
  int high = tree_high(tree);
  printf("数的高度为:%d\r\n", high);

  Tree* copy;
  copy_tree(tree, copy);
  pre_travel(tree);
  printf("\r\n");

  return 0;
}
