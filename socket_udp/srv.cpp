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
  std::string buf;
  struct sockaddr_in addr;

  while(1)
  {
    us.Recv(buf,&addr);
    printf("client say:[%s]\n",buf.c_str());
    printf("server say:");
    fflush(stdout);

    scanf("%s",buf.c_str());
    us.Send(buf,&addr);
  }
  
  us.Close();
  return 0;
}
