
/*
 * 创建任务类
 * 创建线程池类
 * 1.创建线程
 * 2.创建线程安全队列
 * 
 */

#include<iostream>
#include<queue>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>

typedef  bool(*task_callback)(int data);
class Task
{
  public:
    Task()
    {

    }
    Task(int data,task_callback handler)
      :_data(data)
       ,_handler(handler)
    {

    }
    ~Task()
    {

    }
  public:
    //设置任务处理的数据以及处理方法
    void SetTask(int data,task_callback handler)
    {
      _data=data;
      _handler=handler;
    }
    
    //执行任务
    bool Run()
    {
      return _handler(_data);
    }
  private:
    int _data;
    task_callback _handler;
};

#define MAX_THR 5   //线程数
#define MAX_QUE 10  //队列结点数
class ThreadPool
{
  public:
    ThreadPool(int qmax =MAX_QUE,int tmax = MAX_THR)
      :_thr_max(tmax)
       ,_capacity(qmax)
       ,_thr_cur(tmax)
    {
      pthread_mutex_init(&_mutex,NULL);
      pthread_cond_init(&_cond_con,NULL);
      pthread_cond_init(&_cond_pro,NULL);
    }
    ~ThreadPool()
    {
      pthread_mutex_destroy(&_mutex);
      pthread_cond_destroy(&_cond_con);
      pthread_cond_destroy(&_cond_pro);
    }
  public:
    //线程入口函数只能有一个参数，如果为成员函数，则还有一个参数为this指针
    static void *thr_start(void*arg)
    {
      ThreadPool *pool =(ThreadPool*)arg;
      while(1)
      {
        pool->QueueLock();
        while(pool->QueueIsEmpty())
        {
          pool->ConWait();
        }
       Task tt;
       pool->QueuePop(&tt);
       pool->ProWakeUp();
       pool->QueueUnLock();
       tt.Run();   //为了防止处理事件过长导致其线程无法获取锁，因此解锁后再处理
      }
      return NULL;
    }
    bool ThreadPoolInit()
    {
        pthread_t tid;
        int ret,i;
        for(i=0;i<_thr_max;++i)
        {
          ret = pthread_create(&tid,NULL,thr_start,NULL);
          if(ret !=0)
          {
            std::cout<<"thread create error\n";
            return false;
          }
          pthread_detach(tid);
        }
      return true;
    }
    void AddTask(Task tt)
    {
      //向线程池中添加任务
      QueueIsFull();
      while(QueueIsFull())
      {
        ProWait();
      }
      QueuePush(tt);
      ConWakeUp();
      QueueUnLock();
    }
    void ThreadPoolQuit()
    {
      //退出线程池中所有线程
      _quit_flag = true;
     while(_thr_cur > 0)
     {
       ConWakeUpAll();
       
     }
     return ;
    }
  private:
    void QueuePush(Task tt)
    {
      _queue.push(tt);
    }
    void QueuePop(Task *tt)
    {
      *tt = _queue.front();
      _queue.pop();
    }
    void QueueLock()
    {
      pthread_mutex_lock(&_mutex);
    }
    void QueueUnLock()
    {
      pthread_mutex_unlock(&_mutex);
    }
    void ProWait()
    {
      pthread_cond_wait(&_cond_pro,&_mutex);
    }
    void ProWakeUp()
    {
      pthread_cond_signal(&_cond_pro);
    }
    void ConWait()
    {
      //进入这个函数表示现在没有任务
      if(_quit_flag == true)
      {
        //线程池要求退出
        _thr_cur--; //线程数量要减
        std::cout<<"thread:"<<pthread_self()<<"exit\n";
        pthread_mutex_unlock(&_mutex); //退出时解锁
        pthread_exit(NULL);
      }
      pthread_cond_wait(&_cond_con,&_mutex);
    }
    void ConWakeUp()
    {
      pthread_cond_signal(&_cond_con);
    }
    void ConWakeUpAll()
    {
      //广播唤醒所有线程
      pthread_cond_broadcast(&_cond_con);
    }
    bool QueueIsEmpty()
    {
      return _queue.empty();
    }
    bool QueueIsFull()
    {
      return (_queue.size()==_capacity);
    }
  private:
    int _thr_max; //最大线程数量
    int _thr_cur; //当前线程数量  当线程池中线程数量为0时，线程池才可以被销毁
    int _quit_flag;//线程退出标志 线程池中的线程，再退出标志为真，并且没有任务处理的时候退出

  private:
    std::queue<Task> _queue;
    int _capacity;    //队列中的结点数  
    pthread_mutex_t _mutex; //实现互斥

    pthread_cond_t _cond_con;
    pthread_cond_t _cond_pro;

};

bool task_handler(int data)
{
  //休眠一段时间
  srand(time(NULL));
  int sec = rand()%5;

  std::cout<<"thread"<<pthread_self()<<"sleep"<<sec<<"sec\n";
  sleep(sec);
  return true;
}
int main()
{
  ThreadPool pool;
  Task tt[10];
  pool.ThreadPoolInit();
  for(int i=0;i<10;++i)
  {
    tt[i].SetTask(i,task_handler);
    pool.AddTask(tt[i]);
  }
  pool.ThreadPoolQuit();
  return 0;
}
