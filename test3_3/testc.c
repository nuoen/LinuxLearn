#include "stdio.h"

extern void exportfun(char* a);

int main(){
    printf("the main call");
    exportfun("call s file function");
    return 0;
}

int compare(int a,int b){
    if(a>b){
        return a;
    }else{
        return b;
    }
}