#include<iostream>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

using namespace std;

int main(int argc,char*argv[])
{
  if(argc !=3)
  {
    cout<<"./tcp_cil ip port"<<endl;
    return 0;
  }

  string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  int cfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip.c_str());

  connect(cfd,(struct sockaddr*)&addr,sizeof(addr));
  
  while(1)
  {
    string buf;
    printf("cil say:");
    fflush(stdout);
    cin >> buf;
    send(cfd,buf.c_str(),buf.size(),0);

    char s[1024];
    printf("svr say:");
    int size = read(cfd,s,sizeof(s));
    if(size < 0)
    {
      perror("recv");
      return 0;
    }
    else if(size == 0)
    {
      cout<<"peer disconnect"<<endl;
      close(cfd);
      return 0;
    }
    else 
    {
      cout<<buf<<endl;
    }
  }
  return 0;
}
