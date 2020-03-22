#include<stdio.h>
#include<vector>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t lock;     //实现互斥
pthread_cond_t cond; //实现同步
//实现一个生产者消费者模型
//首先得有一个交易场所

std::vector<int> data;

//两个角色，(生产者和消费者)两个线程
//应该使用同步的方式，来进一步改进，
//如果vector中没有数据消费者啥都不做，直到有数据了，才去真正进行消费
void *Product(void* arg)
{
  (void)arg;
  int count=0;
  while(1){
    pthread_mutex_lock(&lock);
    data.push_back(++count);
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);
    usleep(789789);
  }
  return NULL;
}

void *Consume(void* arg)
{
  (void)arg;
  //负责把交易场所中的数据获取出来
  while(1){
    //每次最后一个元素
    //int result = data[data.size()-1];
    pthread_mutex_lock(&lock);
    if(!data.empty()){
      //1.先释放锁
      //2.等待条件就绪(有其它线程调用 pthread_cond_signal)
      //1和2必须是原子性的
      //3.如果条件就绪了，重新获取锁
      //加上wait之后最大的意义在于如果没有数据，消费者线程
      //不必进行空转，节省了资源
      pthread_cond_wait(&cond,&lock);
    }

    int result = data.back();
    data.pop_back();
    printf("result = %d\n",result);
    pthread_mutex_unlock(&lock);
    usleep(123123);
  }
  return NULL;
}

int main()
{
  pthread_mutex_init(&lock,NULL);
  pthread_cond_init(&cond,NULL);

  pthread_t tid1,tid2,tid3,tid4;
  pthread_create(&tid1,NULL,Product,NULL);
  pthread_create(&tid2,NULL,Consume,NULL);
  pthread_create(&tid3,NULL,Product,NULL);
  pthread_create(&tid4,NULL,Consume,NULL);
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
  pthread_join(tid3,NULL);
  pthread_join(tid4,NULL);

  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&cond);
  return 0;
}
