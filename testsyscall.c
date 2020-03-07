#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "arch/x86/include/generated/uapi/asm/unistd_64.h"

int main(int argc, char ** argv) {
	printf("Calling ... \n");
    int length = sizeof(char);
    char temp = 'A';
    printf("Input %s\n", temp);
	syscall(__NR_dm510_msgbox_put, &temp, length);

    char* ret = malloc(length);
	syscall(dm510_msgbox_get, ret, length);

    printf("Ret %s\n", *ret);
}