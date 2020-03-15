#include"tcp.hpp"

int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    printf("./svr ip port");
    return 0 ;
  }
  std::string ip = argv[1];
  uint16_t port =  atoi(argv[2]);

  TcpServer ts;
  if(!ts.CreateSocket())
  {
    return 0;
  }
  if(!ts.Bind(ip,port))
  {
    return 0;
  }
  if(!ts.Listen(5))
  {
    return 0;
  }
  while(1)
  {
    //对端的地址信息
    TcpServer peerts;
    if(!ts.Accept(peerts))
    {
      return 0;
    }
    std::string buf;
    peerts.Rev(buf);
    printf("cli say[%s]\n",buf.c_str());

    printf("svr say:");
    fflush(stdout);
    std::cin>>buf;
    peerts.Send(buf);
  }
  ts.Close();
  return 0;
}
