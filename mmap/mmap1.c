#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/wait.h>

int main()
{
  int *mem = (int*)mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);
  if(mem == MAP_FAILED)
  {
    perror("mmap err");
    return 0;
  }
  pid_t pid =fork();
  if(pid == 0)
  {
    *mem = 101;
    printf("child,*mem=%d\n",*mem);
    sleep(3);
    printf("child,*mem = %d\n",*mem);
  }else if(pid > 0)
  {
    sleep(1);
    printf("parent,*mem = %d\n",*mem);
    *mem = 10001;
    printf("parent,*mem = %d\n",*mem);
    wait(NULL);
  }

  munmap(mem,4);
  return 0;
}
