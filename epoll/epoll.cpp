#include<stdio.h>
#include<unistd.h>
#include<string>
#include<string.h>
#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>

int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    printf("./epoll ip port");
    return 0;
  }

  std::string ip = argv[1]; 
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

  ret = listen(Sock , 36);
  if(ret < 0)
  {
    perror("listen");
    return 0;
  }
  struct sockaddr_in cliaddr;
  socklen_t clilen = sizeof(cliaddr);

  //创建epoll树的根结点
  int epfd = epoll_create(2000);
  //初始化epoll树
  struct epoll_event ev;
  ev.events = EPOLLIN; //读事件
  ev.data.fd = Sock;
  epoll_ctl(epfd,EPOLL_CTL_ADD,Sock,&ev);

  struct epoll_event all[2000];
  while(1)
  {
    //使用epoll通知内核fd做文件的检测
    int ret = epoll_wait(epfd,all,sizeof(all)/sizeof(all[0]),-1);
    //遍历all数组中的前ret个元素
    for(int i = 0; i< ret ;++i)
    {
      int fd  = all[i].data.fd;
      //判断是否有新连接
      if(fd == Sock)
      {
        //接收连接请求
        int cfd = accept(Sock,(struct sockaddr*)&cliaddr,&clilen);
        if(cfd < 0)
        {
          perror("accept");
          return 0;
        }

        //将新得到的cfd挂到epoll树上
        epoll_event temp;
        temp.events = EPOLLIN;
        temp.data.fd = cfd;
        epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&temp);
        printf("New client IP:%s,port:%d\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
      }
      else
      {
        //处理已连接的客户端
        if(!all[i].events & EPOLLIN )
        {
          continue;
        }
        //读数据
        char buf[1024] = {0};
        int len = recv(fd,buf,sizeof(buf),0);
        if(len <0 )
        {
          perror("recv");
          return 0;
        }
        else if(len == 0)
        {
          printf("client disconnect...\n");
          close(fd);
          //将fd从树上删除掉
          epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
        }
        else
        {
          printf("recv buf : %s\n",buf);
          write(fd,buf,len);
        }
      }
    }

  }
  close(Sock);
  return 0;
}
