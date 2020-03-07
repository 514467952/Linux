#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
using namespace std;

template<typename T>
void HalfSort(vector<T> v,T key)
{
  int begin = 0;
  int end = v.size()-1;
  while(begin<end)
  {
    int mid = (begin+end)/2;
    if(v[mid] == key)
    {
      cout<<mid<<endl;
      printf("找到\n");
      exit(0);
    }else if(v[mid > key]){
        end = mid-1;
    }else{
        begin = mid+1;
    }
  }
}

int main()
{
  vector<int> v{3,2,5,8,4,7,6,9};
  sort(v.begin(),v.end());
  HalfSort(v,5);
  return 0;
}






