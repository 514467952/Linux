#pragma once
#include"library.hpp"
#include"student.hpp"

static Student s;


void menu();

void table1();
void table1fun(StudentTable&st,BookTable& bt);

void table2();
void table2fun(StudentTable&st);

void table3();
bool table3fun(StudentTable&st,BookTable& bt);

void table4();
void table4fun1(StudentTable&st,BookTable& bt);
void table4fun2(StudentTable& st);
void table4fun3(StudentTable& st);
void table4fun4(StudentTable& bt);



void menu()
{
  cout<<"===================================="<<endl;
  cout<<"===================================="<<endl;
  cout<<"=========欢迎登陆图书系统==========="<<endl;
  cout<<"===================================="<<endl;
  cout<<"==============1.进入================"<<endl;
  cout<<"==============2.退出================"<<endl;
  cout<<"===================================="<<endl;
}

void table1()
{
  cout<<"===================================="<<endl;
  cout<<"=============1.注册================="<<endl;
  cout<<"=============2.登录================="<<endl;
  cout<<"=============3.退出================="<<endl;
  cout<<"===================================="<<endl;
}
void table1fun(StudentTable&st,BookTable& bt)
{
    int flag2=0;
    cin>>flag2;
    switch (flag2)
    {
    case 1: table2();table2fun(st);break;
    case 2:table3();table3fun(st,bt); break;
    case 3:return ;
    default:
        break;
    }
}


void table2()
{
  cout<<"请输入学号,姓名,班级,密码"<<endl;
}
void table2fun(StudentTable&st)
{
        Student s;
        cin>>s._id>>s._name>>s._class>>s._password;
        st.Insert(&s);
}

void table3()
{
  cout<<"请输入姓名与密码"<<endl;
}
bool table3fun(StudentTable&st,BookTable& bt)
{
    
    cin>>s._name>>s._password;
    if(st.CheckSelf(s))
    {
        while(1)
        {
            table4();
            int flag=0;
            cin>>flag;
            switch (flag)
            {
            case 1:table4fun1(st,bt);break;
            case 2:table4fun2(st);break;
            case 3:table4fun3(st);break;
            case 4:table4fun4(st);break;
            case 5:return true;
            default:cout<<"输入错误请重新输入!"<<endl;
                break;
            }
        }
        return true;
    }
    return false;
}



void table4()
{
  cout<<"=======1.借阅图书==================="<<endl;
  cout<<"=======2.查看借阅图书==============="<<endl;
  cout<<"=======3.归还图书==================="<<endl;
  cout<<"=======4.搜索图书==================="<<endl;
  cout<<"=======5.返回======================="<<endl;
   
}
void table4fun1(StudentTable&st,BookTable& bt)
{
    string bookname;
    int bookid;
    cout<<"请输入书名"<<endl;
    cin>>bookname;
    cout<<"请输入ID"<<endl;
    scanf("%d",&bookid);
    st.StudentBorrow(bookid,&bookname,s);
}

void table4fun2(StudentTable& st)
{

    st.LookBorrow(s._name);
    cout<<endl;

}
void table4fun3(StudentTable& st)
{
    string bookname;
    int bookid;
    cout<<"请输入书名"<<endl;
    cin>>bookname;
    cout<<"请输入ID"<<endl;
    scanf("%d",&bookid);
    st.ReturnBook(bookid,&bookname,s);
}

void table4fun4(StudentTable& st)
{
    string bookname;
    cout<<"请输入书名"<<endl;
    cin>>bookname;
    st.FindBook(&bookname);
}
