#pragma once
#include<stdio.h>
#include<vector>
#include<unistd.h>
#include<pthread.h>
//使用队列来表示生产者与消费者模型
//同步互斥不一定非得用互斥锁和条件变量来实现
//使用信号量来完成
//信号量就是一个计数器，标识可用资源得个数
//P 申请资源，计数器-1
//V 释放资源，计数器+1
//当计数器值为0得时候，再去P操作就会发生阻塞

#include<semaphore.h>

//阻塞队列:一种常见的数据结构，线程安全版本得队列
//
//队列为空，去执行Pop就会阻塞
//如果队列满了，去执行Push也会阻塞
//
//信号量表示互斥很简单，表示同步比较复杂
//要引入两个信号量
//一个信号量表示当前队列中元素的个数
//另外一个表示队列中空格的个数
//插入元素就是在消耗一个空格资源，释放一个元素资源
//删除元素就是消耗了一个元素资源，释放了一个空格资源

template<typename T>
class BlockingQueue{
public:
  BlockingQueue(int max_size)
    :max_size_(max_size)
     ,head_(0)
     ,tail_(0)
     ,size_(0)
     ,queue_(max_size){
    sem_init(&lock_,0,1);
    sem_init(&elem_,0,0);
    sem_init(&blank_,0,max_size);
  }
  ~BlockingQueue(){
    sem_destroy(&lock_);
    sem_destroy(&elem_);
    sem_destroy(&blank_);
  }
  void Push(const T&data){
    //每次插入元素先申请空格资源
    //如果没有空格资源说明队列满了(信号量为0)说明队列满了
    //满了就说明不能继续插入，并且在Push中阻塞
    sem_wait(&blank_);

    sem_wait(&lock_);  //信号量这个计数器的+1-1是原子的
    queue_[tail_]=data;
    ++tail_;
    ++size_;
    sem_post(&lock_);

    sem_post(&elem_);
  }
  void Pop(T* data){
    //每次出队列的时候，就先申请一个元素资源
    //如果每没有元素资源，队列为空
    //就不能直接出队列，则需要在Pop的位置阻塞
    sem_wait(&elem_);

    sem_wait(&lock_);
    *data =queue_[head_];
    ++head_;
    --size_;
    sem_post(&lock_);

    sem_post(&blank_);
  }
private:
  std::vector<T> queue_;
  int head_;
  int tail_;
  int size_;
  int max_size_;

  sem_t lock_;//可以用一个二元信号量(非0即1)表示互斥锁
  sem_t elem_;//可用元素的个数
  sem_t blank_;//可用空格的个数
};
