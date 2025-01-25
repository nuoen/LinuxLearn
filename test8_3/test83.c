#define _GNU_SOURCE
#include "stdio.h"
#include "unistd.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <sched.h>

#define TIME 5
int main(){
    int ret;
    ret = fork();
    if(ret){
        printf("main process runing~\n");
        if(ret<-1){
            printf("fork failed!\n");
        }
        int state;
        pid_t child_pid= wait(&state);
        printf("%d child_pid end!",child_pid);
    }else{
        printf("sub process runing~");
        fflush(stdout);
        while(1){
            sleep(TIME);
            printf("output Oops");
            fflush(stdout);
        }
    }
}