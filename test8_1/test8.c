#define _GNU_SOURCE
#include "stdio.h"
#include "unistd.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <sched.h>

#define STACK_SIZE 1024*1024 //1M
int test_fork(){
    int res = fork();
    printf("call in process : %d\n",getpid());
    printf("fork result is : %d\n",res);
    if(res){
        if(res < -1){
            printf("fork failed");
            return 0;
        }
        //父进程调用
        printf("main fork process success,main is :%d , sub is %d .\n",getpid(),res);
        int status;
        pid_t child_pid = wait(&status);
        if (child_pid > 0) {
            if (WIFEXITED(status)) {
                printf("Child process %d exited with status %d\n", child_pid, WEXITSTATUS(status));
            } else {
                printf("Child process %d did not exit normally\n", child_pid);
            }
        }
    }else{
        //子进程调用
        printf("sub process run");
        int i = 20;
        while(i){
            sleep(1);
            printf("sleep 1 second,last is %d\n",--i);
        }
        exit(90);
    }
    return 0;
}

int child_function(void *arg){
    printf("child thread:PID =%d,arguments =%s\n",getpid(),(char *)arg);
    return 0;
}

void test_clone(){
    char *stack = malloc(STACK_SIZE);
    if(stack == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    char *stack_top = stack+STACK_SIZE;
    //这里传入的stack
    /**
     * 	•	stack 是用 malloc() 分配的连续内存区域，其大小为 STACK_SIZE。
	•	Linux/x86 的函数调用栈是向下生长的，也就是说栈顶在分配区间的最高地址处，然后往下扩展。当我们将 stack + STACK_SIZE 传给 clone() 的第二个参数时，就是告诉内核：“把子线程的栈指针初始设为这块内存的最高地址”。
	•	这样，子线程就有一块独立的栈空间，不会和父线程的栈冲突。
	•	因为使用了 CLONE_VM 等 flag，子线程和父线程共享同一个（大）进程地址空间，但它们各自仍然有“不同的栈指针”。通过给 clone() 提供一个新的栈起始地址，可以避免二者互相干扰。
     */
    pid_t sub_pid = clone(child_function,stack_top,CLONE_VM |
    CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD,"hello, clone");
    if(sub_pid == -1){
        perror("clone");
        free(stack);
        exit(EXIT_FAILURE);
    }
    printf("Parent process: PID = %d, Child PID = %d\n", getpid(), sub_pid);
    // 等待子线程结束
    // sleep(1);
    /** 如果不等待可能会崩溃
     * /mnt/host/test8_1 # ./test8
Parent process: PID = 154, Child PID = 155
child thread:PID =154,arguments =hello, clone
Segmentation fault
/mnt/host/test8_1 # ./test8
child thread:PID =156,arguments =hello, clone
Parent process: PID = 156, Child PID = 157
     */
    // 释放栈内存
    free(stack);

};

void fork_test(){
    int i;
    for(int i=0;i<2;i++){
        fork();
        printf("_\n");
    }
    wait(NULL);
    wait(NULL);
}

static void show_reg(void){
    unsigned int spsr,sp,el;
    asm("mrs %0,spsr_el1":"=r"(spsr) : : "cc");
    asm("mov %0,sp":"=r"(sp)::"cc");
    asm("mrs %0,CurrentEL":"=r"(el)::"cc");
    printf("spsr:0x%x, sp:0x%x,el=%d\n",spsr,sp,el>>2);
}
int global_a = 99;
int main(){
    test_clone();
    fork_test();
    // show_reg();
}