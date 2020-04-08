#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>

using namespace std;

typedef  struct SockInfo
{
  int fd;
  pthread_t tid;
  struct sockaddr_in addr;
}SockInfo;

void* worker(void*arg)
{
  char buf[1024];
  SockInfo* info = (SockInfo*) arg;
  while(1)
  {
    printf("new cil ip:%s,port:%d\n",inet_ntoa(info->addr.sin_addr),ntohs(info->addr.sin_port));
    int size = recv(info->fd,buf,sizeof(buf),0);
    if(size < 0)
    {
      perror("read");
      pthread_exit(NULL);
    }
    else if(size == 0)
    {
      cout<<"客户端断开连接"<<endl;
      close(info->fd);
      pthread_exit(NULL);
    }
    else 
    {
      printf("cil say:%s\n",buf);
      fflush(stdout);

      string s = "你好世界";
      send(info->fd,s.c_str(),s.size(),0);
    }
  }
  return NULL;
}
int main(int argc,char*argv[])
{
  if(argc != 3)
  {
    cout<<"./thread ip port"<<endl;
    return 0;
  }

  string ip =argv[1];
  uint16_t port = atoi(argv[2]);

  int lfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

  int op =1;
  setsockopt(lfd,SO_LINGER,SO_REUSEADDR,&op,sizeof(op));

  struct sockaddr_in addr;
  addr.sin_family =AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip.c_str());
  bind(lfd,(struct sockaddr*)&addr,sizeof(addr));
  
  int ret =listen(lfd,36);
  if(ret < 0)
  {
    perror("listen");
    return 0;
  }
  printf("start accept --\n");

  socklen_t cillen = sizeof(struct sockaddr_in);
  
  SockInfo info[256];
  for(int i =0;i<256;++i)
  {
    info[i].fd = -1;
  }

  while(1)
  {
    int i =0;
    for(;i<256;++i)
      if(info[i].fd == -1)//表明这个位置没用过
        break;
    if(i == 256)
      break;
    
    info[i].fd = accept(lfd,(struct sockaddr*)&info[i].addr,&cillen);
    if(info[i].fd < 0)
    {
      perror("accept");
      return 0;
    }
    pthread_create(&info[i].tid,NULL,worker,&info[i]);
    pthread_detach(info[i].tid);
  }

  close(lfd);
  pthread_exit(NULL);
  return 0;
}
