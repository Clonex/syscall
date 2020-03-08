#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <unistd.h>
#include "arch/x86/include/generated/uapi/asm/unistd_64.h"

int main(int argc, char ** argv) {
	printf("Testing string input! ");
	char str[50] = "This is a stupid message.";
	int length = sizeof(str);

	int inputSize = 10;
	syscall(__NR_dm510_msgbox_put, str, inputSize);
	printf("Input: ");
	for( int i = 0; i < inputSize; i++){
		printf("%c", str[i]);
	}
	printf("\n");

	char* ret = malloc(length);
	syscall(__NR_dm510_msgbox_get, ret, 25);
	printf("Output: %s\n", ret);
	printf("%s\n", ret == "This is a stupid message." ? "PASSED" : "FAILED");
	free(ret);

	printf("Testing input order! \n");

	
	int cLength = sizeof(char);
	syscall(__NR_dm510_msgbox_put, 'A', cLength);
	syscall(__NR_dm510_msgbox_put, 'B', cLength);
	syscall(__NR_dm510_msgbox_put, 'C', cLength);

	ret = malloc(length);
	syscall(__NR_dm510_msgbox_get, ret, 25);

	printf("Input 1 %s\n", *ret == 'A' ? "PASSED" : "FAILED");
	printf("Input 2 %s\n", *ret == 'B' ? "PASSED" : "FAILED");
	printf("Input 3 %s\n", *ret == 'C' ? "PASSED" : "FAILED");
	free(ret);
}
