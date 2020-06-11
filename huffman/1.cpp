#include<iostream>

using namespace std;

template<class W>
struct HuffManTreeNode 
{
  HuffManTreeNode(const W&weight = W())
    :pLeft(nullptr)
     ,pRight(nullptr)
     ,pParent(nullptr)
     ,weight(weight)
  {}

  HuffManTreeNode<W>* pLeft;
  HuffManTreeNode<W>* pRight;
  HuffManTreeNode<W>* pParent;
  W weight;
};

template<class W>
class HuffManTree 
{
  typedef  HuffManTreeNode<W> Node;
  public:
    
  private:
    Node* pRoot;
};
