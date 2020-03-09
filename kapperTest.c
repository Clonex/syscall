#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include "arch/x86/include/generated/uapi/asm/unistd_64.h"

int main(int argc, char ** argv) {
    char str[] = "Hello There!";
    int size = sizeof(str);
    char* ret = malloc(size);
    
    printf("Testing begun!\n First test: putting/getting a message:\n");
    printf("Putting the message 'Hello There!'...\n");
    syscall(__NR_dm510_msgbox_put, str, size);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("Getting a message...\n");
    syscall(__NR_dm510_msgbox_get, ret, size);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tResult of get: %s\n", ret);
    free(ret);

    printf("Next test: order:\n");
    char str1[] = "Test1";
    size = sizeof(str1);
    char* ret1 = malloc(size);

    printf("\tPutting three messages...\n");
    for( int i = 1; i < 4; i++ ){
        syscall(__NR_dm510_msgbox_put, str1, size);
        str1[4] = (char)i;
        printf("\tMsg #%d: %s\n", i, str1);
    }
    
    printf("\tGetting three messages...\n");
    for( int i = 1; i < 4; i++ ){
        syscall(__NR_dm510_msgbox_put, ret1, size);
        printf("\tMsg #%d: %s\n", i, ret1);
    }
    free(ret1);
    
    printf("Next test: putting with different lengths:\n");
    char str2[] = "General Kenobi!";
    size = sizeof(str2);
    char* ret3 = malloc(size);
    
    printf("\tPutting message: 'General Kenobi!' (size = 16) with length = 25...");
    syscall(__NR_dm510_msgbox_put, str2, 25);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tGetting message with length = 16...");
    syscall(__NR_dm510_msgbox_get, ret3, size);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tResult of get: %s\n", ret3);

    printf("\tPutting message: 'General Kenobi!' (size = 16) with length = 10...");
    syscall(__NR_dm510_msgbox_put, str2, 10);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tGetting message with length = 16...");
    syscall(__NR_dm510_msgbox_get, ret3, size);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tResult of get: %s\n", ret3);

    printf("\tPutting message: 'General Kenobi!' (size = 16) with length = 0...");
    syscall(__NR_dm510_msgbox_put, str2, 0);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tGetting message with length = 16...");
    syscall(__NR_dm510_msgbox_get, ret3, size);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tResult of get: %s\n", ret3);

    
    printf("Next test: getting with different lengths:\n");
    
    printf("\tPutting message: 'General Kenobi!' (size = 16) with length = 16...");
    syscall(__NR_dm510_msgbox_put, str2, size);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tGetting message with length = 20...");
    syscall(__NR_dm510_msgbox_get, ret3, 20);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tResult of get: %s\n", ret3);

    printf("\tGetting message with length = 1...");
    syscall(__NR_dm510_msgbox_get, ret3, 1);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tResult of get: %s\n", ret3);

    printf("\tGetting message with length = 0...");
    syscall(__NR_dm510_msgbox_get, ret3, 0);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tResult of get: %s\n", ret3);
    free(ret3);
    
    
    printf("Next test: invalid input:\n");
    char str3[] = "You're a bold one!";
    size = sizeof(str3);
    char* ret4 = malloc(size);

    printf("\tPutting message 'You're a bold one!' (size = 19) with length = -10...");
    syscall(__NR_dm510_msgbox_put, str3, -10);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tGetting a message with length = 19...")
    syscall(__NR_dm510_msgbox_get, ret4, size);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tResult of get: %s\n", ret4);

    printf("\tPutting message 'You're a bold one!' (size = 19) with length = 19...");
    syscall(__NR_dm510_msgbox_put, str3, -10);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tGetting a message with length = -2...")
    syscall(__NR_dm510_msgbox_get, ret4, size);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tResult of get: %s\n", ret4);
    free(ret4);

    char* ret5 = malloc(size);
    printf("Final test: getting from an empty stack:\n");
    printf("\tGetting a message with length = 20...")
    syscall(__NR_dm510_msgbox_get, ret5, 20);
    printf("\tSyscall returned with exitcode: %d, %s\n", (int)errno, strerror(errno));
    printf("\tResult of get: %s\n", ret5);
    free(ret5);

    return 0;
}
