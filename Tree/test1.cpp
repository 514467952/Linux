#include<iostream>
#include<mutex>
using namespace std;

class Singlenton_hungry
{
  private:
    Singlenton_hungry()
    {}
    Singlenton_hungry(Singlenton_hungry const&);
    Singlenton_hungry& operator=(Singlenton_hungry const&);

  public:
    static Singlenton_hungry * getInstace()
    {
      return pSingleton;
    }
  private:
    static Singlenton_hungry* pSingleton;
};

Singlenton_hungry* Singlenton_hungry::pSingleton = new  Singlenton_hungry;

class Singlenton_lazy 
{
  private:
    Singlenton_lazy()
    {}
    Singlenton_lazy(Singlenton_lazy const &);
    Singlenton_lazy & operator = (Singlenton_lazy const&);
  public:
    static Singlenton_lazy* getInstace()
    {
      if(nullptr == pSingleton)
      {
        m_mtx.lock();
        if(pSingleton == NULL)
          pSingleton = new Singlenton_lazy;
        m_mtx.unlock();
      }
        return pSingleton;
    }
  private:
    static Singlenton_lazy* pSingleton;
    static mutex m_mtx;
};

Singlenton_lazy* Singlenton_lazy::pSingleton = NULL;
mutex Singlenton_lazy::m_mtx;
