#include<iostream>
#include<pthread.h>
using namespace std;
class Singleton_hungry{
  private:
    Singleton_hungry(){

    }
    //防止拷贝
    Singleton_hungry( const Singleton_hungry&a);
    Singleton_hungry& operator=(const Singleton_hungry&a);
  public:
    static Singleton_hungry *getInstace()
    {
      return  pSingleton;
    }
  private:
    static Singleton_hungry * pSingleton;

};
//Singleton_hungry * Singleton_hungry::pSingleton = new Singleton_hungry;
Singleton_hungry* Singleton_hungry::pSingleton = new Singleton_hungry;

class A
{
  private:
    A(){};
    A(const A&a);
    A& operator=(const A& a);
  public:
    static A* get()
    {
      return a;
    }
  private:
      static A* a;
};
A* A::a = new A;

class Lock
{
  private:
    pthread_mutex_t myt;
  public:
  Lock(pthread_mutex_t myt):myt(myt)
  {
    pthread_mutex_lock(&myt);
  }
  ~Lock()
  {
    pthread_mutex_unlock(&myt);
  }
};
class B
{
  private:
    B(){};
    B(const B&b);
    B& operator= (const B&b);
  public:
    static B* get()
    {
      if(b == nullptr)
      {
        Lock lock(myt);
        if(b == nullptr)
          b = new B;
      }
      return b;
    }
  private:
    static B* b;
    static pthread_mutex_t myt;
};
B* B::b = NULL;
pthread_mutex_t B::myt = PTHREAD_MUTEX_INITIALIZER;



int main()
{
  
  return 0;
}
