#include<assert.h>
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdio>
#include<cstdlib>
#include<memory>
#include<mysql/mysql.h>

using namespace std;


static MYSQL* MySQLInit()
{
    //初始化mysql句柄，并建立连接
    //1.创建一个句柄
    MYSQL* connect_fd=mysql_init(NULL);
    //2.建立连接数据库
    if(mysql_real_connect(connect_fd,"127.0.0.1","root","123456","address",3306,NULL,0)==NULL)
    {
       printf("连接失败!%s\n",mysql_error(connect_fd));
       mysql_close(connect_fd);

    }
    //3.设定字符编码
    mysql_set_character_set(connect_fd,"utf8");
    return connect_fd;
}

static void MySQLRelease(MYSQL* connect_fd)
{
    //释放句柄，断开连接
    mysql_close(connect_fd);
}


struct Data{
    Data()
    {}
    Data(string name,string address,string tel,string email,int post)
    :usr_name(name)
    ,usr_address(address)
    ,usr_tel(tel)
    ,usr_email(email)
    ,usr_post(post)
    {}
    
    void Show()
    {
        cout<<"name:"<<usr_name<<" "<<"address:"<<usr_address<<" "<<"tel:"<<usr_tel<<" "<<"email:"<<usr_email<<"post:"<<usr_post<<endl;
    }

    string usr_name;
    string usr_address;
    string usr_tel;
    string usr_email;
    int usr_post;
    int usr_id=0;
};

class AddressTable{
public:
    AddressTable(MYSQL* connect_fd)
    :mysql_(connect_fd)
    {
        //通过这个构造函数获取到一个数据库的操作句柄
    }
    bool Insert(Data* value) //添加用户
    {
        unique_ptr<char> sql(new char[1024]);
        sprintf(sql.get(),"insert into addresslist values(%d,'%s','%s','%s','%s',%d)",
           value->usr_id,
           value->usr_name.c_str(),
           value->usr_address.c_str(),
           value->usr_tel.c_str(),
           value->usr_email.c_str(),
           value->usr_post);
        //让数据库服务执行sql
        int ret=mysql_query(mysql_,sql.get());
        if(ret!=0)
        {
          printf("执行插入通讯录失败!%s\n",mysql_error(mysql_));
          return false;
        }
        printf("执行插入结果成功!\n");
        return true;
    }
    bool SelectAll()    //显示用户
    {
      char sql[1024]={0};
      sprintf(sql,"select * from addresslist");
      int ret=mysql_query(mysql_,sql);
      if(ret!=0)
      {
        printf("执行查找通讯录失败!%s\n",mysql_error(mysql_));
        return false;
      }
      printf("执行查找所有用户成功!\n");
      //获取结果集合
      MYSQL_RES* result=mysql_store_result(mysql_);
      //获取每行
      int rows=mysql_num_rows(result);

      for(int i=0;i<rows;i++)
      {
        MYSQL_ROW row=mysql_fetch_row(result);
        printf("序号:%d 姓名:%s 地址:%s 电话:%s 邮箱:%s 邮政编码:%d\n",atoi(row[0]),row[1],row[2],row[3],row[4],atoi(row[5]));

      }
      mysql_free_result(result);
      return true;
    }
    bool Find(string* name)   //查找单个用户
    {
        char sql[1024]={0};
        sprintf(sql,"select address,tel,email,post from addresslist where names='%s'",name->c_str());
        int ret=mysql_query(mysql_,sql);
        if(ret!=0)
        {
          printf("未找到该用户!%s\n",mysql_error(mysql_));
          return false;
        }
        //printf("查找该用户成功!\n");
       //获取结果集合
        MYSQL_RES* result=mysql_store_result(mysql_);
        
      //获取每行
      int rows=mysql_num_rows(result);
      if(rows == 0)
      {
        printf("未找到该用户\n");
        return false;
      }
      for(int i=0;i<rows;i++)
      {
        MYSQL_ROW row=mysql_fetch_row(result);
        printf("地址:%s 电话:%s 邮箱:%s 邮政编码:%d\n",row[0],row[1],row[2],atoi(row[3]));

      }
      return true;
   }
  bool Update(Data* value)   //更新通讯录
  {
      unique_ptr<char> sql(new char[1024]);
      sprintf(sql.get(),"update addresslist set address='%s',tel='%s',email='%s',post=%d where names='%s'",value->usr_address.c_str(),value->usr_tel.c_str(),value->usr_email.c_str(),value->usr_post,value->usr_name.c_str());
      int ret=mysql_query(mysql_,sql.get());
      if(ret!=0)
      {
        printf("更新通讯录失败!%s\n",mysql_error(mysql_));
        return false;
      }
      printf("更新通讯录成功!\n");
      return true;
  }
  bool Sort()           //排序通讯录
  {
    char sql[1024]={0};
    sprintf(sql,"select names,address,tel,email,post from addresslist order by post");
    int ret=mysql_query(mysql_,sql);
    if(ret!=0)
    {
      printf("排序失败!\%s",mysql_error(mysql_));
      return false;
    }
    printf("排序成功!\n");
          //获取结果集合
        MYSQL_RES* result=mysql_store_result(mysql_);
      //获取每行
      int rows=mysql_num_rows(result);

      for(int i=0;i<rows;i++)
      {
        MYSQL_ROW row=mysql_fetch_row(result);
        printf("姓名:%s 地址:%s 电话:%s 邮箱:%s 邮政编码:%d\n",(row[0]),row[1],row[2],row[3],atoi(row[4]));
      }
    return true;
  }

  bool Delete(string * name) //删除
  {
    unique_ptr<char> sql(new char[1024]);
    sprintf(sql.get(),"delete from addresslist where names = '%s'",name->c_str());
    int ret=mysql_query(mysql_,sql.get());
    if(ret!=0)
    {
      printf("删除失败!\%s",mysql_error(mysql_));
      return false;
    }
    printf("删除成功!\n");

    return true;
  }

private:
    MYSQL* mysql_;
};



class AddressList{
public:
    AddressList()
    {}
    void Insert(string name,string address,string tel,string email,int post)
    {
        Data* data=new Data(name,address,tel,email,post);
        al.push_back(data);
    }
    void Print()
    {
        for(size_t i=0;i<al.size();i++)
        {
            al[i]->Show();
        }
    }
    int Find(string name)
    {
        int flag=0;
        for(size_t i=0;i<al.size();i++)
        {
            if(al[i]->usr_name==name)
            {
                al[i]->Show();
                flag=1;
            }
        }
        if(flag==0)
        {
            cout<<"查找的人不存在"<<endl;
            return -1;
        }
        return 1;
    }
    void Update(string name,string address,string tel,string email,int post)
    {
        if(-1==Find(name))
        {
            return ;
        }
        for(size_t i=0;i<al.size();i++)
        {
            if(al[i]->usr_name==name)
            {
                al[i]->usr_address=address;
                al[i]->usr_email=email;
                al[i]->usr_post=post;
                al[i]->usr_tel=tel;
            }
        }            
    }
    void Sort()
    {
        sort(al.begin(),al.end(),[]( Data* p1, Data* p2)
        {
                return p1->usr_post<p2->usr_post;
        });
    }

private:
    vector<Data*> al;
};
