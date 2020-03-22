#include<stdio.h>
//#include<mutex>
#include<unistd.h>
#include<pthread.h>

//互斥量----互斥
pthread_mutex_t mutex;
//条件变量---同步
pthread_cond_t cond;

void * ThreadEntry1(void *arg)
{
  (void)arg;
  while(1){
    printf("传球\n");
    pthread_cond_signal(&cond);
    usleep(789789);
  }
  return NULL;
}

void *ThreadEntry2(void *arg)
{
  (void)arg;
  while(1)
  {
    //执行这个pthread_cond_wait函数就会导致线程被阻塞
    //阻塞到什么时候结束？等待其它线程发送一个通知
    pthread_cond_wait(&cond,&mutex);
    printf("扣篮\n");
    usleep(123456);
  }
  return NULL;
}
int  main()
{
  pthread_mutex_init(&mutex,NULL);
  pthread_cond_init(&cond,NULL);
  pthread_t tid1,tid2;

  pthread_create(&tid1,NULL,ThreadEntry1,NULL);
  pthread_create(&tid2,NULL,ThreadEntry2,NULL);

  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);

 pthread_mutex_destroy(&mutex);
 pthread_cond_destroy(&cond);
  return 0;
}
