#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define DEV_ARR "/dev/idemo"
#define MAX_BUFFER 64

void main(){
    int fd;
    char buffer[MAX_BUFFER];
    int read_ret;
    int write_ret;
    char write_buffer[]="aaabbbbccccc";
    size_t len;
    char *read_buffer;
    len = sizeof(write_buffer);
    fd = open(DEV_ARR,O_RDWR);
    if(fd<0){
        printf("open %s failed\n",DEV_ARR);
        return;
    }
    read_buffer = malloc(2*len);
    read_ret=read(fd,read_buffer,4);
    if(read_ret < 0){
        printf("read failed\n");
        return;
    }
    printf("first read %d bytes\n",read_ret);
    printf("first read %s \n",read_buffer);
    write_ret = write(fd,write_buffer,len);
    close(fd);
    fd = open(DEV_ARR,O_RDWR);
    if(fd<0){
        printf("open %s failed\n",DEV_ARR);
        return;
    }
    read_ret = read(fd,read_buffer,4+len);
    printf("read %d bytes\n",read_ret);
    printf("read %s buffer\n",read_buffer+4);
    for(int i=0;i<2*len;i++){
        printf("read %d,is %c \n",i,*(read_buffer+i));
    }
    close(fd);
    return;
}