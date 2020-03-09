#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include "arch/x86/include/generated/uapi/asm/unistd_64.h"

void get(char* buffer, int size){
    int err = syscall(__NR_dm510_msgbox_get, buffer, size);
    err = err < 0 ? (-1) * err : 0;
    printf("\tExitcode: %d, %s\n", err, strerror(err));
    printf("\tResult of get: %s\n\n", buffer);
}

void put(char* buffer, int size){
    int err = syscall(__NR_dm510_msgbox_put, buffer, size);
    err = err < 0 ? (-1) * err : 0;
    printf("\tExitcode: %d, %s\n\n", err, strerror(err));
}

char* freeAndMalloc(char* buffer, int size){
    free(buffer);
    return (char*)malloc(size);
}

int main(int argc, char ** argv) {
    char str[] = "Hello There!";
    int size = sizeof(str);
    char* ret = malloc(size);
    printf("Testing begun!\nFirst test: putting/getting a message:\n");
    printf("First of all: getting from an empty stack:\n");
    printf("\tGetting a message with buffer size = 20...\n");
    get(ret, size);

    ret = freeAndMalloc(ret, size);
    
    printf("Next test: passing a message:\n");
    printf("Putting the message 'Hello There!'...\n");
    put(str, size);
    printf("Getting a message...\n");
    get(ret, size);
    free(ret);

    printf("Next test: order:\n");
    char str1[] = "Test1";
    size = sizeof(str1);
    ret = malloc(size);

    printf("\tPutting three messages...\n");
    for( int i = 1; i < 4; i++ ){
        sprintf(&str1[4], "%d", i);
        printf("\tMsg #%d: %s\n", i, str1);
        syscall(__NR_dm510_msgbox_put, str1, size);
    }
    
    printf("\tGetting three messages...\n");
    for( int i = 1; i < 4; i++ ){
        syscall(__NR_dm510_msgbox_get, ret, size);
        printf("\tMsg #%d: %s\n", i, ret);
    }
    free(ret);
    
    printf("\nNext test: putting with different buffer sizes:\n");
    char str2[] = "General Kenobi!";
    size = sizeof(str2);
    ret = malloc(size);
    
    printf("\tPutting message: 'General Kenobi!' (size = 16) with buffer size = 16...\n");
    put(str2, 16);
    printf("\tGetting message with buffer size = 16...\n");
    get(ret, 16);
    
    ret = freeAndMalloc(ret, size);

    printf("\tPutting message: 'General Kenobi!' (size = 16) with buffer size = 10...\n");
    put(str2, 10);
    printf("\tGetting message with buffer size = 16...\n");
    get(ret, size);

    ret = freeAndMalloc(ret, size);

    printf("\tPutting message: 'General Kenobi!' (size = 16) with buffer size = 0...\n");
    put(str2, 0);
    printf("\tGetting message with buffer size = 16...\n");
    get(ret, size);

    ret = freeAndMalloc(ret, size);


    printf("Next test: getting with different buffer sizes:\n");
    
    printf("\tPutting message: 'General Kenobi!' (size = 16) with buffer size = 16...\n");
    put(str2, size);
    printf("\tGetting message with buffer size = 20...\n");
    get(ret, 20);

    ret = freeAndMalloc(ret, size);
    
    put(str2, size);
    printf("\tGetting message with buffer size = 16...\n");
    get(ret, 16);

    free(ret);
    
    
    printf("Next test: invalid input:\n");
    char str3[] = "You're a bold one!";
    size = sizeof(str3);
    ret = malloc(size);

    printf("\tPutting message 'You're a bold one!' (size = 19) with buffer size = -10...\n");
    put(str3, -10);

    printf("\tPutting message 'You're a bold one!' (size = 19) with buffer size = 19...\n");
    put(str3, 19);
    printf("\tGetting a message with buffer size = -2...\n");
    get(ret, -2);

    printf("Attempting to get a message with a too small buffer:\n\tGetting a message with buffer size = 5...\n");
    get(ret, 5);
    syscall(__NR_dm510_msgbox_get, ret, size);
    free(ret);

    printf("Testing message passing between multiple processes:\n");
    printf("\tForking...");
    char msg[] = "Hey Parent!";
    size = sizeof(msg);
    __pid_t pid = fork();
    if( pid == 0 ){
        /* Child process */
        printf("\tChild process putting the message: 'Hey Parent!'...\n");
        put(msg, size);
    }
    else if(pid > 0){
        /* Parent process */
        wait(NULL);
        ret = freeAndMalloc(ret, size);
        printf("\tParent process getting a message...\n");
        get(ret, size);
    }

    return 0;
}
