#include<iostream>
#include<unistd.h>
#include<string>
#include<stdio.h>
#include<string.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<arpa/inet.h>

using namespace std;

int main(int argc,char*argv[])
{
  if(argc != 3)
  {
    printf("./epoll3 ip port");
    return 0;
  }

  string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  int Sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(Sock < 0)
  {
    perror("socket");
    return 0;
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip.c_str());

  int ret = bind(Sock,(struct sockaddr*)&addr,sizeof(addr));
  if(ret < 0)
  {
    perror("bind");
    return 0;
  }

  ret = listen(Sock,36);
  if(ret < 0)
  {
    perror("listen");
    return 0;
  }

  struct sockaddr_in ciladdr;
  socklen_t len = sizeof(ciladdr);

  int epollfd = epoll_create(1);
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = Sock;
  epoll_ctl(epollfd,EPOLL_CTL_ADD,Sock,&ev);
  
  struct epoll_event all[200];
  while(1)
  {
    int ret = epoll_wait(epollfd,&ev,sizeof(all)/sizeof(all[0]),-1);
    for(int i=0;i<ret;++i)
    {
      int fd = all[i].data.fd;
      if(fd == Sock)
      {
        int nfd = accept(Sock,(struct sockaddr *)&ciladdr,&len);
        if(nfd < 0)
        {
          perror("accept");
          return 0;
        }

        struct epoll_event tmp;
        tmp.events = EPOLLIN;
        tmp.data.fd =nfd;
        epoll_ctl(Sock,EPOLL_CTL_ADD,nfd,&tmp);
      }
      else 
      {
        if(!all[i].events & EPOLLIN)
        {
          continue;
        }

        char buf[1024];
        int resize = recv(fd,buf,sizeof(buf),0);
        if(resize < 0)
        {
          perror("recv");
          return 0;
        }
        else if(resize == 0)
        {
          printf("cil disconnect");
          epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
        }
        else 
        {
          printf("cil say: %s\n",buf);
          char b[] = "已收到";
          send(fd,b,sizeof(b),0);
          fflush(stdout);
        }
      }
    }
  }
  return 0;
}


