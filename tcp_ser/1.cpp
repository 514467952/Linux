#include<iostream>
#include<stdio.h>    
#include<unistd.h>    
#include<sys/socket.h>    
#include<arpa/inet.h>    
#include<pthread.h>    
using namespace std;    

//自定义数据结构    
//存储线程中的数据    
typedef  struct SockInfo    
{    
  int fd;    
  struct sockaddr_in addr;    
  pthread_t id;    
}SockInfo;    
//子线程处理函数
void* worker(void* arg)
{
  char ip[64];
  char buf[1024];
  SockInfo* info = (SockInfo*)arg;                                                                                                                           
  //通信
  while(1)
  {
    printf("client ip:%s,port %d\n",
        inet_ntop(AF_INET,&info->addr.sin_addr.s_addr,ip,sizeof(ip))
        ,ntohs(info->addr.sin_port));
    int len = read(info->fd,buf,sizeof(buf));
    if(len < 0)
    {
      perror("read");
      pthread_exit(NULL);

    }
    else if(len == 0)
    {
      printf("客户端断开连接\n");
      close(info->fd);
      break;

    }
    else 
    {
      printf("recv buf :%s\n",buf);
      write(info->fd,buf,sizeof(buf));

    }

  }
  return NULL;

}

int main(int argc,char*argv[])
{
  if(argc != 3)
  {
    printf("./thread ip port");
    return 0;

  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  int lfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(lfd < 0)
  {
    perror("socket");
    return 0;

  }

  int op =1;
  setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));

  struct sockaddr_in addr;
  addr.sin_family =AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip.c_str());

  int ret = bind(lfd,(struct sockaddr*)&addr,sizeof(addr));                                                                                                  
  if(ret < 0)
  {
    perror("bind");
    return 0;

  }
  ret = listen(lfd,36);
  if(ret < 0)
  {
    perror("listen");
    return 0;

  }

  socklen_t len = sizeof(struct sockaddr_in);

  int i =0;
  SockInfo info[256]; //创建子线程的个数就限定到了256个
  //规定 fd == -1 
  for( i = 0;i<256; ++i )
  {
    info[i].fd = -1;

  }
  while(1)
  {
    //选择一个没有被使用的，最小的数组元素
    for(i = 0;i < 256;++i)
    {
      if(info[i].fd == -1 )
      {
        break;

      }

    }
    if(i == 256)
    {
      break;

    }

    //主线程---等待接收连接请求
    info[i].fd = accept(lfd,(struct sockaddr*)&info[i].addr,&len);
    //创建子线程 -- 通信
    pthread_create(&info[i].id,NULL,worker,&info[i]);

    //直接线程分离，子线程死亡自动释放
    pthread_detach(info[i].id);

  }
  close(lfd);
  //只退出主线程
  pthread_exit(NULL);
  return 0;

}
