#include<iostream>
#include<vector>
#include<queue>

using namespace std;
template<class W>
struct HuffmanTreeNode
{
  HuffmanTreeNode(const int& weight)
    :_pLeft(nullptr)
     ,_pRight(nullptr)
     ,_pParent(nullptr)
     ,_weight(weight)
  {}
  HuffmanTreeNode<W>* _pLeft;
  HuffmanTreeNode<W>* _pRight;
  HuffmanTreeNode<W>* _pParent;
  W _weight;
};


//以大于的方式比较建立小堆
template<class W>
class Less
{
  typedef  HuffmanTreeNode<W> Node;
  public:
  bool operator()(Node* pLeft,Node* pRight)
  {
    return pLeft->_weight > pRight->_weight;
  }
};

template<class W>
class HuffmanTree
{
  typedef  HuffmanTreeNode<W> Node;
  public:
  HuffmanTree()
    :_pRoot(nullptr)
  {}
  HuffmanTree(const vector<W>& vWeight,const W& invaild)
  {
    CreateHuffmanTree(vWeight,invaild);
  }
  ~HuffmanTree()
  {
    Destroy(_pRoot);
  }
  void CreateHuffmanTree(const vector<W>& vWeight,const W& invaild)
  {
    priority_queue<Node*,vector<Node*>,Less<W>>q;
    for(auto e:vWeight)
    {
      if(e == invaild)
        continue;
      //把数组中的结点放进去
      q.push(new Node(e));
    }

    while(q.size()>1)
    {
      Node* pLeft = q.top();
      q.pop();
      Node* pRight =q.top();
      q.pop();

      Node* pParent = new Node(pLeft->_weight+pRight->_weight);
      pParent->_pLeft = pLeft;
      pParent->_pRight = pRight;

      pLeft->_pParent=pParent;
      pRight->_pParent=pParent;
      q.push(pParent);
    }

    _pRoot = q.top();
  }

  Node* GetRoot()
  {
    return _pRoot;
  }
  private:
  void Destroy(Node* pRoot)
  {
    if(pRoot)
    {
      Destroy(pRoot->_pLeft);
      Destroy(pRoot->_pRight);
      delete  pRoot;
      pRoot = nullptr;
    }
  }
  private:
    Node* _pRoot;
};


#include<string>
#include<algorithm>
#include<stdio.h>
struct CharInfo
{
  unsigned char _ch; //具体的字符
  size_t _count; //字符出现的次数
  string _strCode;//字符的编码

  CharInfo(size_t count = 0)
    :_count(count)
  {}
};

class FileCompree 
{
  public:
    FileCompree()
    {
      _file.resize(256);
      for(int i = 0;i<256;++i)
      {
        _file[i]._ch = i;
        _file[i]._count = 0;
      }
    }
    
    void CompressFile(const string& path)
    {
      //打开压缩文件
      FILE* fIn = fopen(path.c_str(),"rb");
      
      //统计次数
      unsigned char* pReadBuff = new unsigned char[1024];
      int rdSize = 0;
      while(true)
      {
        rdSize = fread(pReadBuff,1,1024,fIn);
        if(0 == rdSize)
          break;
        for(int i =0;i<rdSize;++i)
          _file[pReadBuff[i]]._count++;
      }

      //以这些次数创建Huffman树
      HuffmanTree<CharInfo> t(_file,CharInfo());

      //获取每个字符的编码
      GetFileCompree(t.GetRoot());

      //打开一个文件保存压缩后的结果
      FILE* fOut = fopen("2.txt","wb");

      fseek(fIn,0,SEEK_SET);  //把文件指针放到文件起始位置
      char ch = 0;
      int bitcount = 0;
      while(true)
      {
        rdSize = fread(pReadBuff,1,1024,fIn);
        if(0 == rdSize)
          break;

        for(int i = 0; i< rdSize;++i)
        {
          //拿到编码
          string strCode = _file[pReadBuff[i]]._strCode;

          for(size_t j = 0;j < strCode.size();++j)
          {
            ch <<= 1; 
            if('1' == strCode[i])
              ch |= 1;

            bitcount++;
            if(8 == bitcount)
            {
              fputc(ch,fOut);
              bitcount = 0;
              ch = 0;
            }
          }
        }
      }
      if(bitcount < 8)
      {
        ch <<= 8-bitcount;
        fputc(ch,fOut);
      }
      delete[] pReadBuff;
      fclose(fIn);
      fclose(fOut);
    }
    //计算字符的编码
    void GetFileCompree(HuffmanTreeNode<CharInfo>* pRoot)
    {
      if(nullptr == pRoot)
        return ;
      GetFileCompree(pRoot->_pLeft);
      GetFileCompree(pRoot->_pRight);

      //找到叶子结点
      if(nullptr == pRoot->_pLeft && pRoot->_pRight == nullptr)
      {
        string& strCode = _file[pRoot->_weight._ch]._strCode;
        HuffmanTreeNode<CharInfo>* pCur = pRoot;
        HuffmanTreeNode<CharInfo>* pParent = pCur->_pParent;

        while(pParent)
        {
          if(pCur == pParent->_pLeft)
          {
            strCode +='0';
          }
          else 
          {
            strCode += '1';
          }
          //从叶子结点网上走
          pCur = pParent;
          pParent = pCur->_pParent;
        }
        reverse(strCode.begin(),strCode.end());
      }
    }
  private:
    vector<CharInfo> _file;
};
