#include<cstdio>
#include<unistd.h>
#include<iostream>
#include<string>
#include<sys/select.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

using namespace std;

int main(int argc,char*argv[])
{
  if(argc < 3)
  {
    printf("./select ip port");
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
  if (ret <0)
  {
    perror("bind");
    return 0;
  }
  struct sockaddr_in cliaddr;
  socklen_t clilen = sizeof(cliaddr);

  // 最大文件描述符
  int maxfd = Sock;
  //文件描述符读集合
  fd_set reads,temp;
  //初始化
  FD_ZERO(&reads);
  FD_SET(Sock,&reads);

  while(1)
  {
    //委托内核做IO检测
    temp = reads;
    int ret = select(maxfd+1,&temp,NULL,NULL,NULL);
    if(ret == -1 )
    {
      perror("select");
      return 0;
    }
    // 有客户端发起新连接
    if(FD_ISSET(Sock,&temp))
    {
      //返回值是1，有新连接，接收连接请求
      int cfd = accept(Sock,(struct sockaddr*)&cliaddr,&clilen);
      if(cfd < 0)
      {
        perror("accept");
        return 0;
      }
    
      printf("new clilet IP:%s,port:%d\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
      //将cfd加入到待检测的读集合 ， 下一次就可以检测到了
      FD_SET(cfd,&reads);
      //更新最大的文件描述符
      maxfd = maxfd < cfd ? cfd : maxfd;
    }

    //已经连接的客户端有数据到达
    //轮询遍历
    for(int i = Sock+1;i <= maxfd;++i)
    {
      if(FD_ISSET(i,&temp))
      {
        char buf[1024] = {0};
        int len = recv(i,buf,sizeof(buf),0);
        if(len < 0 )
        {
          perror("recv");
          return 0;
        }
        else if(len == 0)
        {
          printf("客户端已经断开连接\n");
          close(i);
          // 从集合中删除
          FD_CLR(i,&reads);
        }
        else
        {
          printf("rev buf:%s\n",buf);
          send(i,buf,strlen(buf)+1,0);
        }
      }
    }
  }
  close(Sock);
  return 0;
}
