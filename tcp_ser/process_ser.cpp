#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<string>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>
using namespace std;

//回调函数
void recyle(int num)
{
  pid_t pid;
  while((pid) == waitpid(-1,NULL,WNOHANG) > 0)
  {
    printf("child died,pid = %d\n",pid);
  }
}
int main(int argc,char*argv[])
{
  if(argc!=3)
  {
    printf("./process ip port\n");
    return 0;
  }

  string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  int lfd =socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

  //绑定前进行端口复用
  int flag = 1;
  setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));
  
  //绑定地址信息
  struct sockaddr_in addr;
  addr.sin_family =AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip.c_str());

  bind (lfd,(struct sockaddr*)&addr,sizeof(addr));

  listen(lfd,36);

  //使用信号回收子进程PCB
  struct sigaction act;
  act.sa_flags = 0;
  act.sa_handler = recyle;
  sigemptyset(&act.sa_mask);
  sigaction(SIGCHLD,&act,NULL);

  struct sockaddr_in cil_addr;
  socklen_t len = sizeof(cil_addr);
  while(1)
  {
    //父进程接受连接请求
    //accept阻塞的时候被信号中断，处理完信号的操作，回来就不阻塞了直接返回-1
    //errno == EINTR
    int cfd = accept(lfd,(struct sockaddr*)&cil_addr,&len);

    while(cfd < 0 && errno == EINTR)
    {
        cfd = accept(lfd,(struct sockaddr*)&cil_addr,&len);
    }

    // 创建子进程
    pid_t pid = fork();
    if(pid == 0)
    {
      //通信的过程
      while(1)
      {
        char buf[1024];
        int len = read(cfd,buf,sizeof(buf));
        if(len < 0)
        {
          perror("read");
          exit(1);
        }
        else if(len == 0)
        {
          printf("客户端断开连接");
          close(cfd);
          break;
        }
        else 
        {
          printf("recv buf:%s\n" ,buf);
          string s = "";
          printf("server say:");
          scanf("%s",s.c_str());
          send(cfd,s.c_str(),s.size(),0);
        }
      }
      //干掉子进程
      return 0;
    }
    else if(pid > 0) 
    { 
      //父进程
      close(cfd);
    }
  }
  close(lfd);
  return 0;
}
