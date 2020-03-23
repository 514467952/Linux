#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/wait.h>

typedef struct _Student
{
  int sid;
  char name[20];
}Student;

int main(int argc,char*argv[])
{
  if(argc != 2)
  {
    printf("./a.out filname\n");
    return 0;
  }
  
  int fd =open(argv[1],O_RDWR|O_CREAT|O_TRUNC,0666);
  int size = sizeof(Student);
  ftruncate(fd,size);

  Student* stu = (Student*)mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  if(stu == MAP_FAILED)
  {
    perror("mmap err");
    return 0;
  }

  int num =1;
  while(1)
  {
    stu->sid = num;
    sprintf(stu->name,"xiaoming-%03d",num++);
    sleep(1);  //相当于每隔一秒修改映射区的内容
  }

  munmap(stu,size);
  close(fd);
  return 0;
}
