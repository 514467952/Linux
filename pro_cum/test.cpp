#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<vector>
#include<iostream>

using namespace std;

pthread_mutex_t  mutex_;
pthread_cond_t cond_;

vector<int> v;
int count = 1000;
void* Production(void* arg)
{
  (void)arg;
  while(1)
  {
    pthread_mutex_lock(&mutex_);
    v.push_back(++count);
    cout<<"Production result"<<count<<endl;
    pthread_mutex_unlock(&mutex_);
    pthread_cond_signal(&cond_);
    usleep(789789);
  }
  return NULL;
}

void* Consumer(void*arg)
{
  (void)arg;
  while(1)
  {
    pthread_mutex_lock(&mutex_);
    if(v.empty())
    {
      pthread_cond_wait(&cond_,&mutex_);
    }
    int result = v.front();
    v.erase(v.begin());
    cout<<"Consume result == "<< result <<endl;
    pthread_mutex_unlock(&mutex_);
    usleep(123123);
  }
  return NULL;
}

int main()
{
  pthread_mutex_init(&mutex_,NULL);
  pthread_cond_init(&cond_,NULL);

  pthread_t tid[5];
  int i = 0;
  for(;i < 2;++i)
  {
    pthread_create(&tid[i],NULL,Production,NULL);
  }
  for(;i < 5;++i)
  {
    pthread_create(&tid[i],NULL,Consumer,NULL);
  }
  for(int i =0 ;i< 5;++i)
  {
    pthread_join(tid[i],NULL);
  }
  return 0;
}
