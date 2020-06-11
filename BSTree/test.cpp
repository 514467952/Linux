#include<iostream>
using namespace std;

struct BSTreeNode
{
  BSTreeNode(const int val)
    :left(nullptr)
     ,right(nullptr)
     ,val(val)
  {}
  BSTreeNode* left;
  BSTreeNode* right;
  int val;
};

class BSTree
{
  typedef BSTreeNode Node;
  public:
    BSTree()
      :pRoot(nullptr)
    {}
    ~BSTree()
    {
      Destroy(pRoot);
    }
    bool Insert(const int data)
    {
      if(pRoot == nullptr)
      {
        pRoot = new Node(data);
        return true;
      }

      Node* pCur = pRoot;
      Node* pParent = nullptr;
      while(pCur)
      {
        pParent = pCur;
        if(data < pCur->val)
          pCur = pCur->left;
        else if(data > pCur->val)
          pCur  = pCur->right;
        else
          return false;
      }
      pCur = new Node(data);
      if(data < pParent->val)
        pParent->left = pCur;
      else
        pParent->right = pCur;
      return true;
    }
  bool Delete(const int data)
  {
    if(pRoot == nullptr)
      return false;

    Node* pCur = pRoot;
    Node* pParent =nullptr;
    while(pCur)
    {
      if(data < pCur->val)
      {
        pParent =pCur;
        pCur = pCur->left;
      }
      else if(data > pCur->val)
      {
        pParent = pCur;
        pCur = pCur->right;
      }
      else 
        break;
    }
    if(nullptr ==pCur)
      return false;

    Node* pDelNode = pCur;
    if(nullptr == pCur->right)
    {
      if(nullptr == pParent )
      {
        pRoot = pParent->left;
      }

      if(pCur ==  pParent->left)
        pParent->left = pParent->left;
      else 
        pParent->right = pParent->left;
    }
    else if(nullptr == pCur->left)
    {
      if(nullptr == pParent)
      {
        pRoot = pParent->right;
      }
      else 
      {
        if(pCur == pParent->left)
          pParent->left = pCur->right;
        else 
          pParent->right = pCur ->right;
      }
    }
    else 
    {
      Node* pMost = pCur->right;
      Node* pParent = pCur;
      while(pMost -> left)
      {
        pParent = pMost;
        pMost =pMost->left;
      }
      //找到替代结点，把数据换一下
      pCur->val = pMost->val;
      if(pCur == pParent ->left)
        pParent->left = pCur->right;
    }

    delete  pDelNode;
    return true;
  }
  private:
    void Destroy(Node*& pRoot)
    {
      if( nullptr != pRoot )
      {
        Destroy(pRoot->left);
        Destroy(pRoot->right);
        delete  pRoot;
        pRoot = nullptr;
      }
    }
  private:
    Node* pRoot;
};
