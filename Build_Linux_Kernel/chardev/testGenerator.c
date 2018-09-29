#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int main(){
    int ret, fd;
    char* numReceive;
    printf("Starting device test code example...\n");
    
    fd = open("/dev/generDev", O_RDWR); //Open the device to read
    if (fd < 0){
        perror("Failed to open the device..\n");
        return errno;
    }

    printf("Reading from the device...\n"); //Read number from generator
    ret = read(fd, numReceive, 1);
    if (ret < 0){
        perror("Failed to read from the device...\n");
        return errno;
    }

    printf("A received number is %u", numReceive[0]);
    
    printf("\nEnd of the program\n");
    return 0;
}