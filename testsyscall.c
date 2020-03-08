#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <unistd.h>
#include "arch/x86/include/generated/uapi/asm/unistd_64.h"

int main(int argc, char ** argv) {
	printf("Testing ... \n");
    char str[50] = "This is a stupid message.";
    int length = sizeof(str);
    printf("Input: %s\n", str);
    syscall(__NR_dm510_msgbox_put, str, length);

    char* ret = malloc(length);
    syscall(__NR_dm510_msgbox_get, ret, 10);

    printf("Output: %s\n", ret);
}
