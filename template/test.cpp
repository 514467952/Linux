#include<iostream>
#include<string.h>
using namespace std;

//对char和int数组进行排序，排序规则从大到小
template<class T>
void mySort(T arr[],int len)
{
  for(int i = 0; i< len;++i)
  {
    int max = i;
    for(int j = i+1;j<len;++j)
    {
      if(arr[max] < arr[j])
        //交换下标
        max = j;
    }
    if(max != i)
    {
      //交换数据
      swap(arr[max],arr[i]);
    }
  }
}

//输出数组元素模板
template<class T>
void printArray(T arr[],int len)
{
  for(int i = 0;i<len;++i)
    cout<<arr[i]<<" ";
  cout<<endl;
}
void test01()
{
  char charArr[] = "helloworld";
  int num = sizeof(charArr)/sizeof(char);
  mySort(charArr,num);
  printArray(charArr,num);
}
int main()
{
  test01();
  return 0;
}
