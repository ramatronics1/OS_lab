#include<stdio.h>
#include<unistd.h>


int main()
{
    pid_t pid1=fork(),pid2=fork();
    if(pid1==0&&pid2>0){
        sleep(1);
        printf("child 1 and parent are working  ids are %d %d",getpid(),getppid());

    }
    else if(pid2==0&&pid1>0){
           sleep(1);
        printf("child 2 and parent are working  ids are %d %d",getpid(),getppid());

    }
    else if(pid2>0&&pid1==0){
           sleep(1);
        printf("only childen are working  ids are %d %d");


    }
    else{
           sleep(1);
        printf("only parent");
        exit(1);
    }}
