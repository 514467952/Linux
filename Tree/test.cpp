#include<iostream>
using namespace std;

struct TreeNode
{
  int val;
  struct TreeNode* left;
  struct TreeNode* right;
};

//判断两棵树是否相同
bool isSameTree(struct TreeNode* root1,struct TreeNode* root2)
{
  if(root1 == NULL && root2 == NULL)
    return true;
  if(root1 == NULL || root2 == NULL)
    return false;

  return root1->val == root2->val
          &&isSameTree(root1->left,root2->left)
          &&isSameTree(root1->right,root2->right);
}

bool fun(struct TreeNode*p,struct TreeNode*q)
{
  if(p == NULL && q == NULL)
    return true;
  if(p == NULL && q == NULL)
    return false;

  return p->val == q->val
        &&fun(p->left,q->right)
        &&fun(p->right,q->left);
}
//判断是否是对称树
bool isSymmetric(struct TreeNode* root)
{
  if(root == NULL)
    return true;

  return  fun(root->left,root->right);
}



//判断一棵树是另外一棵树的子树
bool isSubstr(struct TreeNode* root,struct TreeNode *t)
{
  if(t == NULL)
    return true;
  if(root == NULL)
    return true;
  if(root->val == t->val && isSameTree(root,t))
    return true;
  bool left = isSubstr(root->left,t);
  if(left == true)
    return true;
  bool right = isSubstr(root->right,t);

  return left || right;
}

//二叉树的最大深度
int maxDepth(struct TreeNode*root)
{
  if(root == NULL)
    return 0;
  int left = maxDepth(root->left);
  int right = maxDepth(root->right);

  return (left >right ? left:right)+1;
}




