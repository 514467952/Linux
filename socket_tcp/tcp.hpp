#include<stdio.h>
#include<unistd.h>
#include<string>
#include<string.h>
#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

class TcpServer
{
  public:
    TcpServer()
    {
      Sock_ = -1;
    }
    ~TcpServer()
    {}
    bool CreateSocket()
    {
      Sock_ = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
      if(Sock_ < 0)
      {
        perror("socket");
        return false;
      }
      return true;
    }
    bool Bind(std::string& ip,uint16_t port)
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      int ret = bind(Sock_,(struct sockaddr*)&addr,sizeof(addr));
      if(ret < 0)
      {
        perror("bind");
        return false;
      }
      return true;
    }    
    bool Listen(int Backlog = 5)
    {
      int ret = listen(Sock_,Backlog);
      if(ret < 0)
      {
        perror("listen");
        return false;
      }
      return true;
    }
    bool Accept(TcpServer& ts, struct sockaddr_in* addr = NULL)
    {
      //1.当前套接字
      //2.对端地址信息
      //3.地址信息长度
      //不用自己填充，accpet会自己填充对端地址信息
      struct sockaddr_in perraddr;
      socklen_t  len = sizeof(struct sockaddr_in);
      int newfd =  accept(Sock_,(struct sockaddr*)&perraddr,&len);
      if(newfd < 0)
      {
        perror("accept");
        return false;
      }
      ts.Sock_ = newfd;
      if(addr !=NULL)
      {
        memcpy(addr,&perraddr,len);
      }
      return true;
    }
    bool Connect(std::string& ip , uint16_t port)
    {
      struct sockaddr_in destaddr;
      destaddr.sin_family = AF_INET;
      destaddr.sin_port = htons(port);
      destaddr.sin_addr.s_addr = inet_addr(ip.c_str());
      int ret = connect(Sock_,(struct sockaddr*)&destaddr,sizeof(destaddr));
      if(ret < 0)
      {
        perror("connect");
        return false;
      }
      return true;
    }
    bool Send(std::string& buf)
    {
      int sendsize = send(Sock_,buf.c_str(),buf.size(),0);
      if(sendsize < 0)
      {
        perror("send");
        return false;
      }
      return true;
    }
    bool Rev(std::string& buffer)
    {
      char buf[1024] = {0};
      //0 阻塞接受
      //MSG_PEEK 阻塞接收
      int recvsize = recv(Sock_,buf,sizeof(buf)-1,0);
      if(recvsize < 0)
      {
        perror("Rev");
        return false;
      }
      else if(recvsize == 0){
          //对端关闭连接
        printf("connect error");
        return false;
      }
      buffer.assign(buf,recvsize);
      return true;
    }
    bool Close()
    {
      close(Sock_);
      Sock_ = -1;
      return true;
    } 
  private:
    int Sock_;
};

