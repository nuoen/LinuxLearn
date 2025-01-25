#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#define GETPUID 457

int main(){
    long pid,uid;
    int ret;
    ret = (int)syscall(GETPUID,&pid,&uid);
    if(ret!=0){
        printf("call getpuid failed");
        return 1;
    }
    printf("call getpuid sucess.\n the pid is %ld, the uid is %ld \n",pid,uid);
    return 0;
}
