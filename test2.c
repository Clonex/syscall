#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <unistd.h>
#include "arch/x86/include/generated/uapi/asm/unistd_64.h"

int main(int argc, char ** argv) {
	printf("Testing ... \n");
	int inputSize = 50;
	/*
	for( int i = 0; i < 5; i++){
	   	char str1[inputSize];
		str1[0] = 'T';
		sprintf(&str1[1], "%d", i);
	    	syscall(__NR_dm510_msgbox_put, str1, inputSize);
	}

       	for(int i = 0; i < 5; i++){
		char* ret = malloc(inputSize);
		syscall(__NR_dm510_msgbox_get, ret, 50);
	    	printf("Output: %s\n", ret);
		free(ret);
	}
	*/
	char test[20] = "Hey bitch hvordan du";
	printf("%ld", syscall(__NR_dm510_msgbox_put, test, 30));
	char* ret = malloc(20);
	printf("%ld", syscall(__NR_dm510_msgbox_get, ret, 25));
	printf("Output: %s\n", ret);
	return 0;
}
