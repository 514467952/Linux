#include<iostream>
#include<algorithm>

using namespace std;
//直接插入
void Insert(int* array,int size)
{
  for(int i = 1; i < size;++i)
  {
    int key = array[i];
    int end = i - 1;
    while(key < array[end]&& end >= 0)
    {
      array[end + 1] = array[end];
      end--;
    }
    array[end+1] = key;
  }
}

//希尔排序
void Shell(int* array,int size)
{
  int gap = size;
  while(gap > 1)
  {
    gap = gap/3 + 1;
    for(int i = 1; i < size;++i)
    {
      int key = array[i];
      int end = i -gap;
      while(key < array[end] && end >= 0)
      {
        array[end+gap] = array[end];
        end -=gap;
      }
      array[end+gap] = key;
    }
  }
}

int main()
{
  int a[] = {3,2,5,8,4,7,6,9};
  Insert(a,8);
  //Shell(a,8);
  for(int i = 0;i < 8;++i)
  {
    cout<<a[i]<<" ";
  }
  cout<<endl;
  return 0;
}
