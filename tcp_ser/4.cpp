#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
using namespace std;

void handler(int sig)
{
  pid_t pid;
  pid  = waitpid(-1,0,WNOHANG);
  cout<<"son dead id:"<<pid<<endl;
  return ;
}
int main(int argc,char*argv[])
{
  if(argc != 3)
  {
    cout<<"./4 ip port"<<endl;
    return 0;
  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  int lfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

  int op = 1;
  setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));

  struct sockaddr_in addr;
  addr.sin_family =AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip.c_str());
  bind(lfd,(struct sockaddr*)&addr,sizeof(addr));

  listen(lfd,36);

  struct sockaddr_in ciladdr;
  socklen_t cillen = sizeof(ciladdr);
  
  signal(SIGCHLD,handler);
  while(1)
  {
    int cfd = accept(lfd,(struct sockaddr*)&ciladdr,&cillen);
    if(cfd < 0 && errno == EINTR)
    {
        cfd =  accept(lfd,(struct sockaddr*)&ciladdr,&cillen);
    }

    pid_t pid = fork();
    if(pid == 0)
    {
      while(1)
      {
        char buf[1024];
        int size = recv(cfd,buf,sizeof(buf),0);
        if(size < 0)
        {
          perror("recv");
          return 0;
        }
        else if(size == 0)
        {
          cout<<"客户端断开连接"<<endl;
          return 0;
        }
        else 
        {
          printf("cil say: %s",buf);
          fflush(stdout);
        
          string s = "已收到";
          send(cfd,s.c_str(),s.size(),0);
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
