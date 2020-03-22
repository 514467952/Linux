#include"tcp.hpp"
int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    printf("./cli [ip] [port]\n");
    return 0;
  }
  std::string ip = argv[1];
  //端口号是一个无符号16位的整数
  uint16_t port = atoi(argv[2]);

  TcpServer ts;
  if(!ts.CreateSocket())
  {
    return 0;
  }

  if(!ts.Connect(ip,port))
  {
    return 0;
  }
  while(1)
  {
    printf("cli say:");
    fflush(stdout);
    std::string buf;
    std::cin>>buf;

    ts.Send(buf);
    ts.Rev(buf);
    printf("svr say:%s\n",buf.c_str());
  }
  ts.Close();
  return 0;
}
