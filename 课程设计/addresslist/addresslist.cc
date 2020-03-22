//#include"addresslist.hpp"
#include"addressfun.hpp"
#include<unistd.h>
#if 0
int main()
{
    AddressList al;
    al.Insert("小明","西安科技大学","123456","1023826776@qq.com",714009);
    al.Insert("小红","西安科技大学","123456","1023826776@qq.com",714008);

    al.Insert("小刚","西安科技大学","123456","1023826776@qq.com",714001);
    al.Print();
    al.Sort();
    cout<<endl;
    al.Print();
    return 0;
}
#endif

#if 0
int main()
{
  MYSQL* mysql=MySQLInit();
  AddressTable al(mysql);
  Data* value=new Data("小红","西安科技大学","13113","012313",714111);
  al.Insert(value);
  return 0;
}
#endif


void menu()
{
  printf("------欢迎使用通讯录-----------\n");
  printf("1.添加用户-------2.删除用户----\n");
  printf("3.查找用户-------4.显示用户----\n");
  printf("5.排序用户-------6.清屏--------\n");
  printf("-----------0.退出--------------\n");
  printf("-------------------------------\n");
}
int main()
{
  while(1)
  {
    sleep(1);
    menu();
    printf("请选择你想要执行的操作\n");
    int n=0;
    scanf("%d",&n);
    switch(n)
    {
      case 0:
        return 0;
        break;
      case 1:
        Insert_1();       
        break;
      case 2:
        Delete_1();
        break;
      case 3:
        Find_1();
        break;
      case 4:
        Show_1();
        break;
      case 5:
        Sort_1();
        break;
      case 6:
        system("clear");
        break;
      default:
        printf("当前执行的操作有误请重新选择\n");
        break;
    }
  }

  printf("感谢你的使用\n");
  return 0;
}

