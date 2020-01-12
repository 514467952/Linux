#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void * ThreadEntry(void *arg)
{
  (void )arg;
  while(1)
  {
    sleep(1);
    int* p =NULL;
    *p =10;
  }
  return NULL;
}

int main()
{
  pthread_t tid;
  pthread_create(&tid,NULL,ThreadEntry,NULL);
  while(1)
  {
    printf("In Main Thread\n");
    sleep(1);
  }
}
