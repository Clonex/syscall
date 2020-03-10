#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
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

/* Empty stack */
void test1(int size){
    char* ret = malloc(size);
    printf("Test1: getting from an empty stack:\n");
    printf("\tGetting a message with buffer size = 20...\n");
    get(ret, size);
    free(ret);
}

/* Passing a message */
void test2(char* src, int size){
    char* ret = malloc(size);
    printf("Test2: passing a message:\n");
    printf("Putting the message '%s'...\n", src);
    put(src, size);
    printf("Getting a message...\n");
    get(ret, size);
    free(ret);
}

/* Order of the stack */
void test3(){
    printf("Test3: order of the stack:\n");
    char str1[] = "Test1";
    int size = sizeof(str1);
    char* ret = malloc(size);

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
}

/* Putting with different buffer sizes */
void test4(char* src, int size){
    printf("\nTest4: putting with different buffer sizes:\n");
    char* ret = malloc(size);
    
    printf("\tPutting message: '%s' (size = %d) with buffer size = 16...\n", src, size);
    put(src, 16);
    printf("\tGetting message with buffer size = 16...\n");
    get(ret, 16);
    
    ret = freeAndMalloc(ret, size);

    printf("\tPutting message: '%s' (size = %d) with buffer size = 10...\n", src, size);
    put(src, 10);
    printf("\tGetting message with buffer size = 16...\n");
    get(ret, size);

    ret = freeAndMalloc(ret, size);

    printf("\tPutting message: '%s' (size = %d) with buffer size = 0...\n", src, size);
    put(src, 0);
    printf("\tGetting message with buffer size = 16...\n");
    get(ret, size);
    free(ret);
}

/* Getting with different buffer sizes */
void test5(char* src, int size){
    printf("Test5: getting with different buffer sizes:\n");
    char* ret = malloc(size);
    
    printf("\tPutting message: '%s' (size = %d) with buffer size = 16...\n", src, size);
    put(src, size);
    printf("\tGetting message with buffer size = 20...\n");
    get(ret, 20);

    ret = freeAndMalloc(ret, size);

    printf("\tPutting message: '%s' (size = %d) with buffer size = 16...\n", src, size);
    put(src, size);
    printf("\tGetting message with buffer size = 16...\n");
    get(ret, 16);

    free(ret);
}

/* Too small buffer */
void test6(char* src, int size){
    char* ret = malloc(size);
    printf("\tPutting message: '%s' (size = %d) with buffer size = 16...\n", src, size);
    put(src, size);
    printf("Test7: Attempting to get a message with a too small buffer:\n\tGetting a message with buffer size = 5...\n");
    get(ret, 5);
    syscall(__NR_dm510_msgbox_get, ret, size);
    free(ret);
}

/* Invalid input */
void test7(char* src, int size){
    printf("Test7: invalid input:\n");
    char* ret = malloc(size);

    printf("\tPutting message '%s' (size = %d) with buffer size = -10...\n", src, size);
    put(src, -10);

    ret = freeAndMalloc(ret, size);

    printf("\tPutting message '%s' (size = %d) with buffer size = 19...\n", src, size);
    put(src, 19);
    printf("\tGetting a message with buffer size = -2...\n");
    get(ret, -2);
    free(ret);
}

/* Two processes */
void test8(){
    printf("Testing message passing between multiple processes:\n");
    char msg[] = "Hey Parent!";
    int size = sizeof(msg);
    char* ret = malloc(size);

    printf("\tForking...\n");
    __pid_t pid = fork();
    if( pid == 0 ){
        /* Child process */
        printf("\tChild process putting the message: '%s'...\n", msg);
        put(msg, size);
    }
    else if(pid > 0){
        /* Parent process */
        wait(NULL);
        printf("\tParent process getting a message...\n");
        get(ret, size);
        free(ret);
    }
}

int main(int argc, char ** argv) {
    if(argc != 1){
        printf("Wrong number of arguments. Aborting...\n");
        return 0;
    }
    int test = atoi(argv[1]);
    if(test < 1 || test > 8){
        printf("Not a valid test. Aborting...\n");
        return 0;
    }
    printf("Testing begun!\n\n");
    char str1[] = "Hello There!";
    int size1 = sizeof(str1);
    char str2[] = "General Kenobi!";
    int size2 = sizeof(str2);
    char str3[] = "You're a bold one!";
    int size3 = sizeof(str3);
    switch(test){
        case 1:
            test1(size1);
            break;
        case 2:
            test2(str1, size1);
            break;
        case 3:
            test3();
            break;
        case 4:
            test4(str2, size2);
            break;
        case 5:
            test5(str2, size2);
            break;
        case 6:
            test6(str2, size2);
            break;
        case 7:
            test7(str3, size3);
            break;
        case 8:
            test8();
            break;
    }
    
    return 0;
}