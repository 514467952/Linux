#include<stdio.h>
#include<unistd.h>
#include<malloc.h>
#include<pthread.h>

#define SIZE 10000000
//假设存在一个很大的数组，把数组中的每个元素都进行一个乘方运算，再赋值回数组中。

void Calc(int* arr,int beg,int end)
{
  for(int i= beg;i< end;++i)
  {
    arr[i] = arr[i]*arr[i];
  }
}


int main()
{
  int* arr=(int*)malloc(sizeof(int)*SIZE);
  pthread_t tid;
  pthread_create(&tid,NULL,ThreadEntry,NULL);
  while(1);
  return 0;
}
