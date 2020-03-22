#include<iostream>
#include<stack>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#include<assert.h>
using namespace std;

void bubblesort(int *array,int size)
{
  
  for(int i = 0; i<size-1;++i)  //趟数
  {
    int flag = 0;
    for(int j = 0;j<size-i-1;++j)
    {
      if(array[j] > array[j+1])
      {
        swap(array[j],array[j+1]);
        flag = 1;
      }
    }
    if(!flag)
      return;
  }
}

int Partion(int*array ,int left,int right)
{
  int num = array[right-1];
  int begin = left;
  int end = right-1;
  while(begin < end)
  {
    while(begin<end && array[begin]<=num)
      begin++;
    while(begin<end && array[end] >= num)
      end--;
    if(begin<end)
      swap(array[end],array[begin]);
  }
  if(begin != right-1)
    swap(array[begin],array[right-1]);
  return begin;
}

void QuickSort(int*array,int left,int right)
{
  if(right - left >1)
  {
    int div = Partion(array,left,right);
    QuickSort(array,left,div);
    QuickSort(array,div+1,right);
  }
}
//快速排序非递归
void QuickSortNor(int*array,int size)
{
  int left = 0;
  int right = size;
  stack<int>s;
  s.push(right);
  s.push(left);
  while(!s.empty())
  {
      left = s.top();
      s.pop();
      right = s.top();
      s.pop();
      if(left<right)
      {
        int div = Partion(array,left,right);
        s.push(right);
        s.push(div+1);
        s.push(div);
        s.push(left);
      }
  }
}


//归并排序
void MergeData(int* array,int left,int mid,int right,int *temp)
{
  int begin1 = left; int end1 = mid;
  int begin2 = mid;int end2 = right;
  int index = left;
  while(begin1<end1 && begin2<end2)
  {
    if(array[begin1] < array[begin2])
      temp[index++] = array[begin1++];
    else
      temp[index++] = array[begin2++];
  }
  while(begin1<end1)
    temp[index++] = array[begin1++];
  while(begin2<end2)
    temp[index++] = array[begin2++];
}

void _MergeSort(int* array,int left,int right,int*temp)
{
  if((right - left) >1)
  {
    int mid = left + ((right-left)>>1);
    _MergeSort(array,left,mid,temp);
    _MergeSort(array,mid,right,temp);
    MergeData(array,left,mid,right,temp);
    memcpy(array+left,temp+left,sizeof(array[left])*(right-left));
  }
}

void MergeSort(int*array,int size)
{
  int* temp = new int[size*sizeof(array[0])];

  _MergeSort(array,0,size,temp);
  delete[] temp;
}
int main()
{
  int a[] = {3,2,5,8,4,7,6,9};

  //bubblesort(a,8);
  //QuickSort(a,0,8);
  //QuickSortNor(a,8);
  MergeSort(a,8);
  for(auto e : a)
  {
    cout<<e<<" ";
  }
  cout<<endl;
  return 0;
}
