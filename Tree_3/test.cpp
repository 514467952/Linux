#include<iostream>
using namespace std;

struct TreeNode
{
  struct TreeNode* left;
  struct TreeNode* right;
  int val;
};

//二叉树的最近公共祖先
TreeNode* LaterParent(struct TreeNode* root,struct TreeNode* p,struct TreeNode* q)
{
  if(p == NULL || q == NULL || root == NULL)
    return root;
  TreeNode* left = LaterParent(root->left,p,q);
  TreeNode* right =  LaterParent(root->right,p,q);
  
  return left == nullptr? right:(right == nullptr?left:root);
}

//前序中序重构二叉树
struct TreeNode* buildTree(int* preorder, int preorderSize, int* inorder, int inorderSize)
{
  if(preorder == 0 || inorder == 0)
    return NULL;
  int index = -1;
  for(int i = 0;i < inorderSize; ++i)
  {
    if(inorder[i] == preorder[0])
      index = i;
  }
  if(index == -1)
    return NULL;

  int rootvalue = preorder[0];
  struct TreeNode* root =(struct TreeNode*)malloc(sizeof(struct TreeNode));
  root->val = rootvalue;
  root->left = buildTree(preorder+1,index,inorder,index);
  root->right = buildTree(preorder+1+index,preorderSize-index-1,inorder+index+1,inorderSize-index-1);

  return root;
}



#include<assert.h>
struct TreeNode* buildTree2(int* preorder,int preorderSize,int*inorder,int inorderSize)
{
  assert(preorderSize == inorderSize);
  if(preorderSize == 0 || inorderSize == 0)
    return NULL;
  int rootvalue = preorder[0];
  int index = -1;
  for(int i = 0;i < inorderSize;++i)
  {
    if(inorder[i] == rootvalue)
      index = i;
  }
  if(index == -1)
    return NULL;

  struct TreeNode* root = (struct TreeNode*)malloc(sizeof(struct TreeNode));
  root->val = rootvalue;
  root->left = buildTree2(preorder+1,index,inorder,index);
  root->right = buildTree2(preorder+1+index,preorderSize-index,inorder+index+1,inorderSize-index-1);
  return root;
}














