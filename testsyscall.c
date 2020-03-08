#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <unistd.h>
#include "arch/x86/include/generated/uapi/asm/unistd_64.h"

int main(int argc, char ** argv) {
	printf("Testing ... \n");
	char str[50] = "This is a stupid message.";
   	int length = sizeof(str);

	int inputSize = 10;
    	syscall(__NR_dm510_msgbox_put, str, inputSize);
    	printf("Input: ");
	for( int i = 0; i < inputSize; i++){
		printf("%c", str[i]);
	}
	printf("\n");


//    	for(int i = 0; i < 10; i++){
		char* ret = malloc(length);
		syscall(__NR_dm510_msgbox_get, ret, 25);
	    	printf("Output: %s\n", ret);
		free(ret);
//	}
}
