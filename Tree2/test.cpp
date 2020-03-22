//二叉搜索树转为排序得双向链表
#include<iostream>
using namespace std;
struct TreeNode
{
  struct TreeNode* left;
  struct TreeNode* right;
  int val;
};

class Partion
{
  public:
    TreeNode* prev = NULL;
    TreeNode* first;
    void fun(struct TreeNode* node)
    {
      node->left = prev;
      if(prev != NULL)
      {
        prev->right = node;
      }
      else
      {
        first = node;
      }
      prev = node;
    }

    void Inorder(struct TreeNode* root)
    {
      if(root == NULL)
        return ;
      Inorder(root->left);
      fun(root);
      Inorder(root->right);
    }

    TreeNode* Create(struct TreeNode*root)
    {
      Inorder(root);
       return first;
    }
};
