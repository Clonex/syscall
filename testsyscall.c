#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include "arch/x86/include/generated/uapi/asm/unistd_64.h"

int main(int argc, char ** argv) {
	printf("Running tests!\n");
	char str[] = "This is a stupid message.";
	int length = sizeof(str);

	syscall(__NR_dm510_msgbox_put, str, length);

	char* ret = malloc(length);
	syscall(__NR_dm510_msgbox_get, ret, length);
	printf("Input 0: %s\n", strcmp(ret, "This is a stupid message.") == 0 ? "\033[0;32mPASSED\033[0m;" : "\033[0;31mFAILED\033[0m;");
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
	printf("Input 1 %s\n", *ret == 'C' ? "\033[0;32mPASSED\033[0m;" : "\033[0;31mFAILED\033[0m;");

	syscall(__NR_dm510_msgbox_get, ret, cLength);
	printf("Input 2 %s\n", *ret == 'B' ? "\033[0;32mPASSED\033[0m;" : "\033[0;31mFAILED\033[0m;");

	syscall(__NR_dm510_msgbox_get, ret, cLength);
	printf("Input 3 %s\n", *ret == 'A' ? "\033[0;32mPASSED\033[0m;" : "\033[0;31mFAILED\033[0m;");
	free(ret);
}
