#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<iostream>
#include<queue>

#define  THREADCOUNT  4 //要初始化多少线程
typedef void(*Handler_t)(int);


//任务类
class ThreadTask
{
public:
  ThreadTask()
  {
    Data_ = -1;
    Handler_ = NULL;
  }

  ThreadTask(int Data,Handler_t Handler)
  {
    Data_=Data;
    Handler_ = Handler;
  }

  void Run()
  {
    Handler_(Data_);
  }
private:
  int Data_;
  Handler_t Handler_;
};

class ThreadPool
{
public:
  ThreadPool()
  { 
      ThreadCapacity_ = THREADCOUNT;
      //创建线程时进行统计线程数量
      ThreadCurNum_ = THREADCOUNT;

      pthread_mutex_init(&Mutex_,NULL);
      pthread_cond_init(&Cond_,NULL);

      //创建线程
      bool IsCreate = ThreadCreate();
      if(!IsCreate)
      {
        printf("ThreadPool Create thread false\n");
        exit(1);
      }
  }

  //放数据
  //放的就是任务
  bool Push(ThreadTask* Tt)
  {
    pthread_mutex_lock(&Mutex_);
    //如果设置了线程当中的线程进行退出，则就不要再进行push数据了，将互斥锁释放，让线程池当中的线程退出
    if(IsQuit_)
    {
      pthread_mutex_unlock(&Mutex_);
      return false;
    }
    Que_.push(Tt);
    pthread_mutex_unlock(&Mutex_);

    //放完数据的时候要通知线程池当中的线程
    pthread_cond_signal(&Cond_);
    return true;
  }

  //取数据
  //要把队列中的数据返回出去，要用二级指针，因为*Tt是数据本身，返回要返回数据的地址，这样在外部才能拿到数据
  bool Pop(ThreadTask** Tt)
  {
    *Tt = Que_.front();
    Que_.pop();
    return true;
  }

  //清理线程池
  bool PoolClear()
  {
    pthread_mutex_lock(&Mutex_);
    IsQuit_ = true;
    pthread_mutex_unlock(&Mutex_);

    //当线程池当中的队列没有数据的时候，线程都是阻塞再pthread_cond_wait当中，等待被唤醒
    //通知一次即可
    if(ThreadCurNum_ > 0)
    {
      pthread_cond_broadcast(&Cond_);
    }
    return true;
  }
private:
  //线程的入口函数
  //为什么要定义为static,因为隐蔽掉this指针，在Linux线程库中对线程入口函数的参数定义中没有this指针
  static void *ThreadStart(void* arg)
  {
    ThreadPool* tp = (ThreadPool*)arg;//拿到线程池当中的this指针，才能操作线程池中的成员函数和变量_
    while(1)
    {
      pthread_mutex_lock(&tp->Mutex_);
      //if(IsQuit) //线程不能再此处退出
      while(tp->Que_.empty())
      {
        //如果线程要退出
        if(tp->IsQuit_)
        {
          tp->ThreadQuit();
        }
        //如果队列为空，则等待线程
        pthread_cond_wait(&tp->Cond_,&tp->Mutex_);
      }
      ThreadTask* tt;
      tp->Pop(&tt);
      pthread_mutex_unlock(&tp->Mutex_);
      
      //为什么在解锁后run，由于已经拿到数据了，所以可以把锁释放掉了 
      tt->Run();
    }
    return NULL;
  }

  //线程退出函数
  void ThreadQuit()
  {
    //临界资源
    ThreadCurNum_--;
    //如果线程退出时候可能没解锁,又因为ThreadCurNum也是一个临界资源，所以，在--完后再进行解锁
    pthread_mutex_unlock(&Mutex_);
    pthread_exit(NULL);
  }
private:
  //创建线程
  bool ThreadCreate()
  {
    int ret = -1;
    pthread_t tid;
    for(int i = 0;i < THREADCOUNT; ++i)
    {
      //传this指针为了访问队列
      ret = pthread_create(&tid,NULL,ThreadStart,(void*)this);
      //printf("%d\n,i");
      if(ret != 0)
      {
        printf("create thread failed!\n");
        return false;
      }
    }
    return true;
  }
private:
  //线程池当中 初始化的时候，线程的数量
  size_t ThreadCapacity_;
  //当前线程池有多少个数量
  size_t ThreadCurNum_;
  //线程安全的队列
  std::queue<ThreadTask*> Que_;
  //要实现线程安全必须要互斥
  pthread_mutex_t Mutex_;
  //要实现同步
  pthread_cond_t Cond_;

  //线程退出的标志
  bool IsQuit_;
};

//告诉线程拿到数据如何处理
void Printf(int Data)
{
  printf("i am Handler func, i print [%d]\n",Data);
}
int main()
{
  ThreadPool* tp =new ThreadPool();
  for(int i = 0; i< 10; ++i)
  {
    ThreadTask* tt =new ThreadTask(i,Printf);
    tp->Push(tt);
  }
  
  sleep(6);
  tp->PoolClear();
  delete tp;
  return 0;
}
