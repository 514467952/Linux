#include<iostream>
#include<string>
#include<algorithm>
using namespace std;
string str = "" ;
//long long num;
void test(int n,int k = 2)
{
  if(n == 0)
    str += "0";
  while(n)
  {
    str += (n%k)+'0';
    n = n/k;
  }
  reverse(str.begin(),str.end());
}

int main(int argc,char*argv[])
{
  int n ;
  cin>>n;
  test(n);
  
  cout<<str<<endl;
  return 0;
}
