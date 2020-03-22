#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
//1.线程之间共享虚拟地址空间

int g_count=0;

void * ThreadEntry(void *arg)
{
  (void )arg;
 // int count =10;
  while(1)
  {
    printf("In ThreadEntry\n");
    ++g_count;
    sleep(1);
  }
}

int main()
{
  pthread_t tid;
  pthread_create(&tid,NULL,ThreadEntry,NULL);
  pthread_detach(tid);
  while(1)
  {
    printf("In MainThread %d\n",g_count);
    sleep(1);
  }
  return 0;
}
