#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>
using namespace std;

void sign0(int num)
{
  pid_t pid;
  while((pid) == waitpid(-1,NULL,WNOHANG) > 0)
  {
    printf("cil dead id : %d\n",pid);
  }
}
int main(int argc,char*argv[])
{
  if(argc != 3)
  {
    printf("./test ip port");
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
  int op = 1;
  setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));
  
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
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

  struct sigaction act;
  act.sa_flags = 0;
  act.sa_handler = sign0;
  sigemptyset(&act.sa_mask); //清空信号屏蔽
  //sigaction(SIGCLD,&act,NULL);

  struct sockaddr_in ciladdr;
  socklen_t len = sizeof(ciladdr);

  while(1)
  {
    int cfd = accept(lfd,(struct sockaddr*)&ciladdr,&len);
    if(cfd < 0 && errno == EINTR)
    {
       cfd = accept(lfd,(struct sockaddr*)&ciladdr,&len);
    }

    pid_t pid = fork();
    if(pid == 0)
    {
      while(1)
      {
        char buf[1024];
        int rec = read(cfd,buf,sizeof(buf));
        if(rec < 0)
        {
          perror("read");
          exit(1);
        }
        else if(rec == 0)
        {
          cout<<"客户端断开连接"<<endl;
          close(cfd);
          break;
        }
        else 
        {
          printf("cil say:%s\n",buf);
          string s = "已接收到信息";
          write(cfd,s.c_str(),s.size());
        }
      }
    }
    else if(pid > 0)
    {
      close(cfd);
    }
  }
  close(lfd);
  return 0;
}
