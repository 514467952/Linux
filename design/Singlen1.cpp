#include<iostream>

using namespace std;

class A
{
  public:
    static A* fun()
    {
      return  a;
    }
  private:
    static A* a;
  private:
    A(const A& a);
    A& operator= (const A&a);
};

ss Singleton_hungry{
      private:
        Singleton_hungry(){
                
        }
              //防止拷贝
              //      Singleton_hungry(Singleton const&);
              //          Singleton_hungry& operator=(Singleton const&);
              //              public:
              //                    static Singleton_hungry *getInstace()
              //                          {
              //                                  return  pSingleton;
              //                                        }
              //                                          //实现内嵌垃圾回收类
              //                                              #if 0
              //                                                    class Garbo{
              //                                                            ~Garbo(){
              //                                                                      if (pSingleton != NULL){
              //                                                                                  delete pSingleton;
              //                                                                                            }
              //                                                                                                    }
              //                                                                                                          };
              //                                                                                                              #endif
              //                                                                                                                  private:
              //                                                                                                                        static Singleton_hungry * pSingleton;
              //                                                                                                                            };
              //                                                                                                                                Singleton_hungry * Singleton_hungry::pSingleton = new Singleton_hungry;
              //
              //                                                                      }
              //                                                            }
              //                                                    }
              //                          }
}
