#include<iostream>
#include<stdio.h>
using namespace std;
int main()
{
  while(1)
  {
    FILE* fp = fopen("1.txt","wb");
    string buf =  "abcdefg";
    fwrite(buf.c_str(),1,buf.size(),fp);
    fclose(fp);
  }
  return 0;
}
