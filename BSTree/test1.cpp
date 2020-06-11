#include<iostream>

using namespace std;

template<class W>
struct HuffmanTreeNode
{
  HuffmanTreeNode(const W&val)
    :pLeft(nullptr)
     ,pRight(nullptr)
     ,pParent(nullptr)
     ,_val(val)
  {}
  struct HuffmanTreeNode<W> * pLeft;
  struct HuffmanTreeNode<W> * pRight;
  struct HuffmanTreeNode<W> * pParent;
  W _val;
};

template<class W>
class Less 
{
  public:
  typedef  HuffmanTreeNode<W> Node;
  int operator()(const Node* p,const Node* q)
  {
    return p->_val > q->_val;
  }
};
