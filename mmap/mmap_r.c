#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/wait.h>

typedef  struct _Student{
  int sid;
  char name[20];
}Student;
int main(int argc,char*argv[])
{
  int fd = open(argv[1],O_RDWR);
  int length = sizeof(Student);
  Student* stu = (Student*)mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  if(stu == MAP_FAILED)
  {
    perror("mmap err");
    return 0;
  }

  while(1)
  {
    printf("sid = %d,sname = %s\n",stu->sid,stu->name);
    sleep(1);
  }
  munmap(stu,length);
  close(fd);
  return 0;
}
