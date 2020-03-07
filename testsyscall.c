#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <unistd.h>
#include "arch/x86/include/generated/uapi/asm/unistd_64.h"

int main(int argc, char ** argv) {
	printf("Testing ... \n");
    int length = sizeof(char);
    char temp = 'A';
    printf("Input %c\n", &temp);
	syscall(__NR_dm510_msgbox_put, &temp, length);

    char* ret = malloc(length);
	syscall(__NR_dm510_msgbox_get, ret, length);

    printf("Ret %c\n", ret);
}