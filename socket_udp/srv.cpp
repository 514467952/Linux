#include"udp.hpp"
#include<stdlib.h>
int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    printf("./srv [ip] [port]");
    return 0;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  UdpSrv us;
  if(!us.CreateSocket())
  {
    return 0;
  }
  if(!us.Bind(ip,port))
  {
    return 0;
  }

  while(1)
  {
    std::string buf;
    //数据来源的信息
    struct sockaddr_in addr;
    us.Recv(buf,&addr);
    printf("client say:[%s]\n",buf.c_str());
    
    printf("server say:");
    fflush(stdout);

    std:: cin>> buf;
    us.Send(buf,&addr);
  }
  
  us.Close();
  return 0;
}
