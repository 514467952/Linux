#include<iostream>
#include<stack>
using namespace std;

struct TreeNode 
{
  TreeNode* left;
  TreeNode* right;
  int val;
};

typedef  TreeNode Node;
void NorPreorder(Node* root)
{
  stack<Node*> s;
  s.push(root);

  Node* pCur = root;
  while(!s.empty() || pCur !=nullptr)
  {
    while(pCur != nullptr)
    {
      cout << pCur->val<<" ";
      s.push(pCur);
      pCur = pCur->left;
    }

    Node* top = s.top();
    s.pop();

    pCur = top->right;
  }
  cout << endl;
}


