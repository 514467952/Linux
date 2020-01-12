#include<stdio.h>
#include<unistd.h>
#include<pthread.h>


void * ThreadEntry(void *arg)
{
  (void )arg;
 // int count =10;
  while(1)
  {
    printf("In ThreadEntry\n");
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
    printf("In MainThread\n");
    sleep(1);
    pthread_cancel(tid);
  }
  return 0;
}
