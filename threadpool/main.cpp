#include"threadpool.hpp"

bool handler(int data)
{
  srand(time(NULL));
  int n = rand()%5;
  printf("线程ID:%lu Run Task:%d -- sleep %d 秒\n",pthread_self(),data,n);
  sleep(n);
  return true;
}

int main()
{
  int i;
  ThreadPool pool;
  pool.PoolInit();
  for(i=0;i<10;++i)  //新建10个任务
  {
    Task* tt = new Task(i,handler);
    pool.PushTask(tt);
  }

  pool.PoolQuit();
  return 0;
}
