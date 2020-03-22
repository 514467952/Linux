#include<iostream>
#include<algorithm>
using namespace std;

void Select(int* array,int size)
{
  int begin = 0;
  int end = size-1;
  while(begin < end)
  {
    int max = begin;
    int min = begin;
    int i = begin;
    while(i <= end)
    {
      if(array[i]>array[max])
        max = i;
      if(array[i]<array[min])
        min = i;
      ++i;
    }
    if(max  != end)
      swap(array[max],array[end]);
    if(min == end)
      min = max;
    if(min != begin)
      swap(array[min],array[begin]);
    begin++;
    end--;
  }
}

void AdjustDown(int*array,int size,int parent)
{
  int child = 2*parent+1;
  while(child<size)
  {
    if(child+1 <size && array[child+1] > array[child])
      child+=1;
    if(array[parent] < array[child])
    {
      swap(array[parent],array[child]);
      parent = child;
      child = 2*parent+1;
    }
    else
      return ;
  }
}
void HeapSort(int *array,int size)
{
  for(int i = (size-2)/2;i>=0;)
    AdjustDown(array,size,i--);

  int end = size-1;
  while(end)
  {
    swap(array[0],array[end]);
    AdjustDown(array,end,0);
    end--;
  }
}
int main()
{
  int a[] = {3,2,5,8,4,7,6,9};
  //Select(a,8);
  HeapSort(a,8);
  for(int i = 0;i<8;++i)
  {
    cout<<a[i]<<" ";
  }
  cout<<endl;
  return 0;
}
