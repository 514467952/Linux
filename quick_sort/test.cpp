#include<iostream>

using namespace std;
int a[]={3,2,5,8,4,7,6,9};

void quick_sort(int a[],int l,int r)
{
  if(l>=r) return;
  int num =a[(r+l)>>1];
  int i = l-1;
  int j = r+1;
  while(i<j)
  {
    do i++;while(a[i]<num);
    do j--;while(a[j]>num);
    if(i<j) swap(a[i],a[j]);
  }
  quick_sort(a,l,j);
  quick_sort(a,j+1,r);
}
int main()
{
  quick_sort(a,0,7);
  for(int i = 0;i<7;++i)
    cout<<a[i]<<" ";
  cout<<endl;
  return 0;
}
