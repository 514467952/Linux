#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<signal.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/wait.h>
using namespace std;

void sign0(int num)
{
  pid_t pid;
  while((pid) == waitpid(-1,NULL,WNOHANG)>0 )
  {
    cout<<"子进程死亡id："<<pid<<endl;
  } 
}
int main(int argc,char*argv[])
{
  if(argc != 3)
  {
    cout<<"./3 ip port"<<endl;
    return 0;
  }

  string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  int lfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  int op =1;
  setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));

  struct sockaddr_in addr;
  addr.sin_family =AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip.c_str());
  bind(lfd,(struct sockaddr*)&addr,sizeof(addr));

  listen(lfd,36);
  cout<<"start accept"<<endl;

  struct sigaction act;
  act.sa_flags = 0;
  act.sa_handler = sign0;
  sigemptyset(&act.sa_mask);
  sigaction(SIGCHLD,&act,NULL);

  struct sockaddr_in ciladdr;
  socklen_t cillen = sizeof(ciladdr);

  while(1)
  {
    int cfd = accept(lfd,(struct sockaddr*)&ciladdr,&cillen);
    if(cfd < 0 && errno == EINTR)
    {
        cfd = accept(lfd,(struct sockaddr*)&ciladdr,&cillen);  
    }

    printf("new cilent ip:%s,port:%d\n",inet_ntoa(ciladdr.sin_addr),ntohs(ciladdr.sin_port));
    pid_t pid = fork();
    if(pid == 0)
    {
      while(1)
      {
        char buf[1024];
        int size = read(cfd,buf,sizeof(buf));
        if(size < 0)
        {
         perror("recv");
          exit(1);
        }
        else if(size == 0)
        {
          cout<<"客户端断开连接"<<endl;
          close(cfd);
          exit(1);
        }
        else 
        {
          printf("cil say :%s\n",buf);
          string s;
          cin>>s;
          write(cfd,s.c_str(),s.size());
        }
      }
    }
    else if(pid > 0)
    {
      //父进程关闭cfd，因为cfd已经在子进程中进行通信了
      close(cfd);
    }
  }

  close(lfd);
  return 0;
}
