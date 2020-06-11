#include<iostream>
#include<pthread.h>
#include<queue>
#include<unistd.h>
#define  MAX_THREAD_NUM 5
typedef bool (*handler_t)(int);

class Task{
  public:
    Task()
      :data(-1)
       ,handler(NULL)
    {}

    Task(int data ,handler_t handler)
      :data(data)
       ,handler(handler)
    {}

    void SetTask(int data,handler_t handler)
    {
      this->data = data;
      this->handler = handler;
    }
    void Run()
    {
      handler(data);
    }
  private:
    int data;
    handler_t handler;
};

class ThreadPool {
  private:
    int thread_max_num; //最大线程数
    int thread_num; //当前线程数
    bool thread_quit; //线程退出标志
    std::queue<Task*> task_queue; //任务队列
    pthread_mutex_t mytx;
    pthread_cond_t cond;
  private:
    static void* thr_start(void* arg)
    {
     ThreadPool* tp = (ThreadPool*) arg;
     while(1)
     {
       pthread_mutex_lock(&tp->mytx);

       while(tp->task_queue.empty())
       {
         if(tp->thread_quit)
         {
          tp->thread_num--;
          pthread_mutex_unlock(&tp->mytx);
         }
         pthread_cond_wait(&tp->cond,&tp->mytx);
       }
       
       Task* tt;
       tp->PopTask(&tt);
       pthread_mutex_unlock(&tp->mytx);
       pthread_mutex_unlock(&tp->mytx);
       tt->Run();
       delete  tt;
     }
     return NULL;
    }
  public:
    ThreadPool(int max = MAX_THREAD_NUM)
      :thread_max_num (max) 
       , thread_num (max)
    {
      pthread_mutex_init(&mytx,NULL);
      pthread_cond_init(&cond,NULL);
    }
    ~ThreadPool()
    {
      pthread_mutex_destroy(&mytx);
      pthread_cond_destroy(&cond);
    }

    bool PoolInit()
    {
      pthread_t tid;
      for(int i=0;i<thread_max_num;++i)
      {
        int ret = pthread_create(&tid,NULL,thr_start,this);
        if(ret != 0 )
        {
          std::cout<<"thread create false"<<std::endl;
          return false;
        }
      }
      return true;
    }
    
    bool PushTask(Task* tt)
    {
      pthread_mutex_lock(&mytx);
      if(thread_quit)
      {
        pthread_mutex_unlock(&mytx);
      }
      task_queue.push(tt);
      pthread_cond_signal(&cond); //唤醒一个线程去工作
      pthread_mutex_unlock(&mytx);
      return true;
    }

    bool PopTask(Task** tt)  //让线程取从队列中取出任务
    {
      *tt = task_queue.front();
      task_queue.pop();
      return true;
    }

    bool PoolQuit()
    {
      pthread_mutex_lock(&mytx);
      thread_quit = true;
      pthread_mutex_unlock(&mytx);
      while(thread_num > 0)
      {
        pthread_cond_broadcast(&cond);
        usleep(1000);
      }
      return true;
    }
};
