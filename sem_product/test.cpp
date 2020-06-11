#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
using namespace std;
sem_t blank;//生产者
sem_t full;//消费者
#define size 5

int buf[size];
int beginnum = 100;
void *thr_producter(void*arg)
{
  int i = 0;
  while(1)
  {
    sem_wait(&blank);
    cout<<"thr_producter---"<<beginnum<<endl;
    buf[(i++)%size] = beginnum++;
    sem_post(&full);
    sleep(rand()%3);
  }
  return NULL;
}
void *thr_customer(void*arg)
{
  int i = 0;
  int num = 0;
  while(1)
  {
    sem_wait(&full);
    num = buf[(i++)%size];
    cout<<"thr_customer------"<< num <<endl;
    sem_post(&blank);
    sleep(rand()%3);
  }
  return NULL;
}

int main()
{
  sem_init(&blank,0,size);
  sem_init(&full,0,0);

  pthread_t tid[2];
  pthread_create(&tid[0],NULL,thr_producter,NULL);
  pthread_create(&tid[0],NULL,thr_customer,NULL);

  pthread_join(tid[0],NULL);
  pthread_join(tid[1],NULL);
  sem_destroy(&blank);
  sem_destroy(&full);
  return 0;
}
