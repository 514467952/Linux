#include<iostream>
using namespace std;

template<class T>
class BSNode
{
  //T()设置默认值
  //对于内置类型，为0
  //对于自定义类型，调用构造
  public:
    BSNode(const T& data = T())
      :left_(nullptr)
      ,right_(nullptr)
      ,data_(data)
    {}
    BSNode<T>* left_;
    BSNode<T>* right_;
    T data_;
};

template<class T>
class BSTree
{
  typedef BSNode<T> Node;
  public:
    BSTree()
      :pRoot(nullptr)
    {}
    ~BSTree()
    {
      _Destroy(pRoot);
    }
    //插入
    bool Insert(const T& data)
    {
      if(pRoot == nullptr)
        return false;
      Node* pCur = pRoot;
      Node* pParent = nullptr;
      while(pCur)
      {
        pParent = pCur;
        if(data == pCur->data_)
          break;
        else if(data < pCur->data_)
          pCur = pCur->left_;
        else
          pCur = pCur->right_;
      }

      pCur = new Node(data);
      if(data < pParent->left_)
        pParent->left_ = pCur;
      else
        pParent->right_ = pCur;
    }

    //查找
    Node* Find(const T& data)
    {
      Node* pCur = pRoot;
      Node* pParent = nullptr;
      while(pCur)
      {
        if(data == pCur->data_)
          return pCur;
        else if(data < pCur->data_)
          pCur = pCur->left_;
        else
          pCur = pCur->right_;
      }
      return nullptr;
    }
    //获取最左侧结点
    Node* LeftMost()
    {
      if(pRoot == nullptr)
        return nullptr;
      Node* pCur =pRoot;
      while(pCur->left_)
        pCur = pCur->left_;

      return pCur;
    }
    //获取最右侧结点
    Node* RightMost()
    {
      if(pRoot == nullptr)
        return nullptr;
      Node* pCur = pRoot;
      while(pCur)
        pCur = pCur->right_;
      return pCur;
    }

    //删除结点
    bool Delete(const T& data)
    {
      if(pRoot == nullptr)
      {
        cout<<"为空，不删除"<<endl;
        return false;
      }
      //找到待删除的结点的位置
      Node* pCur = pRoot;
      Node* pParent = nullptr;
      while(pCur)
      {
        pParent = pCur;
        if(data == pCur->data_)
          break;
        else if(data < pCur->data_)
          pCur = pCur->left_;
        else if(data > pCur->data_)
          pCur = pCur->right_;
      }

      if(pCur == nullptr)
      {
        cout<<"未找到，不删除"<<endl;
        return false;
      }

      //找到了，进行删除
      Node* pDelNode = pCur;
      if(pDelNode->right_ == nullptr) //只有左孩子或者叶子结点
      {
        if(pParent == nullptr)
        {
          pRoot = pCur->left_;
        }
        else
        {
          if(pCur == pParent->left_) //如果pCur是双亲的左结点
            pParent->left_ = pCur->left_;
          else                        //如果pCur是双亲的右结点
            pParent->right_ = pCur->left_;
        }
      }
      else if(pDelNode->left_ == nullptr)
      {
        if(pParent == nullptr)
          pRoot = pCur->left_;
        else
        {
          if(pCur == pParent->left)
            pParent->left_ = pCur->left_;
          else
            pParent->right_ = pCur->right_;
        }
      }
      else
      {
        //待删除结点左右子树均存在，
        //在其右子树中找一个最小的
        Node* pMost  = pCur->left_;
        pParent = pCur;
        while(pMost->left_)
        {
          pParent = pMost;
          pMost = pMost->left_;
        }
        //找到最小，只要交换数据，就不用删除原来应该删掉的结点，因为数据交换了，只要删除
        //被交换的结点就可以了
        pCur->data_ = pMost->data_;

        if(pMost == pParent ->left_)
          pParent ->left_ = pMost->right_;
        else
          pParent ->right_ = pMost->left_;

        pDelNode = pMost;
      }
      delete  pDelNode;
      return true;
    }
  private:
    //中序遍历验证二叉搜索树
    void InOrder(Node* pRoot)
    {
      if(pRoot)
      {
        InOrder(pRoot->left_);
        cout<<pRoot->data_<<endl;
        InOrder(pRoot->right_);
      }
    }
    void _Destroy(Node* pRoot)
    {
      if(pRoot)
      {
        _Destroy(pRoot->left_);
        _Destroy(pRoot->right_);
        delete pRoot;
        pRoot = nullptr;
      }
    }
  private:
    Node* pRoot;
};

int main()
{
  return 0;
}
