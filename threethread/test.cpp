#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>


int g_num;
pthread_mutex_t mutex;
pthread_cond_t cond1,cond2,cond3;

void* Func_p1(void* arg)
{
  while(1)
  {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond1,&mutex);
    printf("---thread id: %lu , p1 num: %d \n",pthread_self(),g_num);
    pthread_mutex_unlock(&mutex);
    sleep(1);

  }
  return NULL;

}

void* Func_p2(void* arg)
{
  while(1)
  {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond2,&mutex);
    printf("---thread id: %lu , p2 num: %d \n",pthread_self(),g_num);
    pthread_mutex_unlock(&mutex);
    sleep(1);

  }
  return NULL;

}

void* Func_p3(void* arg)
{
  while(1)
  {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond3,&mutex);
    printf("---thread id: %lu , p3 num: %d \n",pthread_self(),g_num);
    pthread_mutex_unlock(&mutex);
    sleep(1);

  }
  return NULL;

}

void* Func_p4(void* arg)
{
  while(1)
  {
    pthread_mutex_lock(&mutex);
    ++g_num;
    pthread_mutex_unlock(&mutex);
    if((g_num % 3) == 0)
      pthread_cond_signal(&cond3);
    else if((g_num % 3) == 1)
      pthread_cond_signal(&cond1);
    else if((g_num % 3) == 2)
      pthread_cond_signal(&cond2);
    sleep(1);

  }
  return NULL;

}

int main()
{
  pthread_t p1,p2,p3,p4;

  pthread_mutex_init(&mutex,NULL);
  pthread_cond_init(&cond1,NULL);
  pthread_cond_init(&cond2,NULL);
  pthread_cond_init(&cond3,NULL); 

  pthread_create(&p1,NULL,Func_p1,NULL);
  pthread_create(&p2,NULL,Func_p2,NULL);
  pthread_create(&p3,NULL,Func_p3,NULL);
  pthread_create(&p4,NULL,Func_p4,NULL);

  pthread_join(p1,NULL);
  pthread_join(p2,NULL);
  pthread_join(p3,NULL);
  pthread_join(p4,NULL);

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond1);
  pthread_cond_destroy(&cond2);
  pthread_cond_destroy(&cond3);

  return 0;

}

