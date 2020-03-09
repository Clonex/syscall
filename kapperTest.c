#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include "arch/x86/include/generated/uapi/asm/unistd_64.h"

void get(char* buffer, int length){
    syscall(__NR_dm510_msgbox_get, buffer, length);
    printf("\tExitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tResult of get: %s\n\n", buffer);
}

void put(char* buffer, int length){
    syscall(__NR_dm510_msgbox_put, buffer, length);
    printf("\tExitcode: %d, %s\n\n", (int)errno, strerror(errno));
}

char* freeAndMalloc(char*, size ;buffer, int size){
    free(buffer);
    return (char*)malloc(size);
}

int main(int argc, char ** argv) {
    char str[] = "Hello There!";
    int size = sizeof(str);
    char* ret = malloc(size);
    
    printf("Testing begun!\nFirst test: putting/getting a message:\n");
    printf("Putting the message 'Hello There!'...\n");
    put(str, size);
    printf("Getting a message...\n");
    get(ret, size);
    free(ret);

    printf("Next test: order:\n");
    char str1[] = "Test1";
    size = sizeof(str1);
    char* ret1 = malloc(size);

    printf("\tPutting three messages...\n");
    for( int i = 1; i < 4; i++ ){
        sprintf(&str1[4], "%d", i);
        printf("\tMsg #%d: %s\n", i, str1);
        syscall(__NR_dm510_msgbox_put, str1, size);
    }
    
    printf("\tGetting three messages...\n");
    for( int i = 1; i < 4; i++ ){
        syscall(__NR_dm510_msgbox_get, ret1, size);
        printf("\tMsg #%d: %s\n", i, ret1);
    }
    free(ret1);
    
    printf("\nNext test: putting with different lengths:\n");
    char str2[] = "General Kenobi!";
    size = sizeof(str2);
    char* ret30 = malloc(size);
    
    printf("\tPutting message: 'General Kenobi!' (size = 16) with length = 25...\n");
    put(str2, 25);
    printf("\tGetting message with length = 16...\n");
    get(ret30, 16);
    
    char* ret31 = freeAndMalloc(ret30, size);

    printf("\tPutting message: 'General Kenobi!' (size = 16) with length = 10...\n");
    put(str2, 10);
    printf("\tGetting message with length = 16...\n");
    get(ret31, size);

    char* ret32 = freeAndMalloc(ret31, size);

    printf("\tPutting message: 'General Kenobi!' (size = 16) with length = 0...\n");
    put(str2, 0);
    printf("\tGetting message with length = 16...\n");
    get(ret32, size);

    char* ret40 = freeAndMalloc(ret32, size);

    printf("Next test: getting with different lengths:\n");
    
    printf("\tPutting message: 'General Kenobi!' (size = 16) with length = 16...\n");
    put(str2, size);
    printf("\tGetting message with length = 20...\n");
    get(ret40, 20);

    char* ret41 = freeAndMalloc(ret40, size);
    
    printf("\tGetting message with length = 1...\n");
    get(ret41, 1);

    char* ret42 = freeAndMalloc(ret41, size);
    
    printf("\tGetting message with length = 0...\n");
    get(ret42, 0);

    free(ret42);
    
    
    printf("Next test: invalid input:\n");
    char str3[] = "You're a bold one!";
    size = sizeof(str3);
    char* ret4 = malloc(size);

    printf("\tPutting message 'You're a bold one!' (size = 19) with length = -10...\n");
    put(str3, -10);
    printf("\tGetting a message with length = 19...\n");
    get(ret4, size);

    printf("\tPutting message 'You're a bold one!' (size = 19) with length = 19...\n");
    put(str3, 19);
    printf("\tGetting a message with length = -2...\n");
    get(ret4, -2);

    free(ret4);

    char* ret5 = malloc(size);
    printf("Final test: getting from an empty stack:\n");
    printf("\tGetting a message with length = 20...\n");
    get(ret5, 20);

    free(ret5);

    return 0;
}
