#include<stdio.h>
#include<unistd.h>
#include<string>
#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
using namespace std;

int main(int argc ,char* argv[])
{
  if(argc != 3)
  {
    printf("./epoll2.cpp ip,port");
    return 0;
  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  int sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(sock < 0)
  {
    perror("socket");
    return 0;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip.c_str());
  int ret = bind(sock,(struct sockaddr*)&addr,sizeof(addr));
  if(ret < 0)
  {
    perror("perror");
    return 0;
  }
  ret = listen(sock,36);
  if(ret <  0)
  {
    perror("listen");
    return 0;
  }
  
  struct sockaddr_in ciladdr;
  socklen_t cillen = sizeof(ciladdr);
  
  //创建epoll根结点
  int epfd = epoll_create(2000);
  if(epfd < 0)
  {
    perror("create");
    return 0;
  }
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = sock;

  epoll_ctl(epfd,EPOLL_CTL_ADD,sock,&ev);

  epoll_event all[2000];
  while(1)
  {
    int ret = epoll_wait(epfd,all,sizeof(all)/sizeof(all[0]),-1);
    for(int i = 0; i< ret; ++i)
    {
      int fd = all[i].data.fd;
      if(fd == sock)
      {
        int cfd = accept(fd,(struct sockaddr*)&ciladdr,&cillen);
        if(cfd < 0)
        {
          perror("accept");
          return 0;
        }
        epoll_event temp;
        temp.data.fd =cfd;
        temp.events = EPOLLIN;
        epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&temp);
      }
      else 
      {
        char buf[1024] = {0};
        int recvsize = recv(fd,buf,sizeof(buf),0);
        if(recvsize <0 )
        {
          perror("recv");
          return 0;
        }
        else if(recvsize == 0)
        {
          close(fd);
          epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&ev);
          printf("cli disconnect\n");
        }
        else
        {
          printf("cli say: %s",buf);
          write(fd,buf,sizeof(buf));
        }
      }
    }
  }
  return 0;
}

