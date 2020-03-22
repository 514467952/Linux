#pragma once
#include"addresslist.hpp"
#include<string>
#include<iostream>

void Insert_1()
{
  MYSQL * mysql = MySQLInit();
  AddressTable add(mysql);

  printf("请输入你联系人信息\n");
  string usr_name;
  string usr_address;
  string usr_tel;
  string usr_email;
  int post = 0;
  printf("请输入联系人姓名\n");
  std::cin>>usr_name;
  printf("请输入联系人地址\n");
  std::cin>>usr_address;
  printf("请输入联系人电话\n");
  std::cin>>usr_tel;
  printf("请输入联系人电子邮箱\n");
  std::cin>>usr_email;
  printf("请输入联系人邮箱\n");
  std::cin>>post;
  
  Data *value= new Data(usr_name,usr_address,usr_tel,usr_email,post);
  add.Insert(value);

  MySQLRelease(mysql);
  delete value;
}

void Show_1()  //全部用户
{
  MYSQL * mysql = MySQLInit();
  AddressTable add(mysql);
  
  add.SelectAll();
  MySQLRelease(mysql);
}

void Find_1() //查找单个用户
{
  MYSQL * mysql = MySQLInit();
  AddressTable add(mysql);

  printf("请输入你想要查找的用户的姓名\n");
  string name;
  std::cin>>name;

  add.Find(&name);
  MySQLRelease(mysql);
}

void Sort_1()
{

  MYSQL * mysql = MySQLInit();
  AddressTable add(mysql);

  add.Sort();
  MySQLRelease(mysql);
}

void Delete_1()
{
  MYSQL * mysql = MySQLInit();
  AddressTable add(mysql);
  printf("请输入你想要删除联系人的名字\n");
  string name;
  std::cin>>name;
  bool ret = add.Find(&name);
  if(!ret)
  {
    printf("没有此人,无法删除\n");
    return ;
  }

  add.Delete(&name);
  //MySQLRelease(mysql);
}
