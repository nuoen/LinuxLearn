#include <sys/resource.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int pid = getpid();
    int my_priority = getpriority(PRIO_PROCESS,0);
    printf("the current process priority is %d\n",my_priority);
    int i=-20;
    while (my_priority<19)
    {
        int ret =setpriority(PRIO_PROCESS,0,i);
        if(ret<0){
            perror("setpriority failed");
            exit(errno);
        }
       my_priority = getpriority(PRIO_PROCESS,pid); 
        printf("set %d, the process priority is %d\n",i,my_priority);

       i++;
    }
    exit(EXIT_SUCCESS);
}