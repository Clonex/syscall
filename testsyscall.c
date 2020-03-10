#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include "arch/x86/include/generated/uapi/asm/unistd_64.h"

int main(int argc, char ** argv) {
	printf("Running tests!\n");
	char successMsg[] = "\033[0;32mPASSED\033[0m";
	char failedMsg[] = "\033[0;31mFAILED\033[0m";

	char str[] = "This is a stupid message.";
	int length = sizeof(str);

	syscall(__NR_dm510_msgbox_put, str, length);

	char* ret = malloc(length);
	syscall(__NR_dm510_msgbox_get, ret, length);
	printf("[Input / Output]\tTest 0: %s\n", strcmp(ret, str) == 0 ? successMsg : failedMsg);
	free(ret);

	int cLength = sizeof(char);
	char temp = 'A';
	syscall(__NR_dm510_msgbox_put, &temp, cLength);
	temp = 'B';
	syscall(__NR_dm510_msgbox_put, &temp, cLength);
	temp = 'C';
	syscall(__NR_dm510_msgbox_put, &temp, cLength);

	ret = malloc(length);
	syscall(__NR_dm510_msgbox_get, ret, cLength);
	printf("[Input order]\t\tTest 1: %s\n", *ret == 'C' ? successMsg : failedMsg);

	syscall(__NR_dm510_msgbox_get, ret, cLength);
	printf("[Input order]\t\tTest 2: %s\n", *ret == 'B' ? successMsg : failedMsg);

	syscall(__NR_dm510_msgbox_get, ret, cLength);
	printf("[Input order]\t\tTest 3: %s\n", *ret == 'A' ? successMsg : failedMsg);

	int out = syscall(__NR_dm510_msgbox_get, ret, cLength);
	printf("[Empty stack]\t\tTest 4: %s\n", out < 0 && abs(out) == ENODATA ? successMsg : failedMsg);

	out = syscall(__NR_dm510_msgbox_get, ret, -1);
	printf("[Invalid get length]\tTest 5: %s\n", out < 0 && abs(out) == EINVAL ? successMsg : failedMsg);

	out = syscall(__NR_dm510_msgbox_put, ret, -1);
	printf("[Invalid put length]\tTest 6: %s\n", out < 0 && abs(out) == EINVAL ? successMsg : failedMsg);
	free(ret);

	__pid_t pid = fork();
	if( pid == 0 ){
       syscall(__NR_dm510_msgbox_put, str, length);
    }
    else if(pid > 0){
        /* Parent process */
        wait(NULL);
		char* rData = malloc(length);
		syscall(__NR_dm510_msgbox_get, rData, length);
		printf("[Multiple proccesses]\tTest 8: %s\n", strcmp(rData, str) == 0 ? successMsg : failedMsg);
        free(rData);
    }
}