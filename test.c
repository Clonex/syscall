#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "arch/x86/include/generated/uapi/asm/unistd_64.h"
#include <stdbool.h>

/* Test trying to get from an empty stack */
void testEmptyStack(void){
  char* buffer = malloc(20 * sizeof(char));
  int errCode = syscall(__NR_dm510_msgbox_get, buffer, 20 * sizeof(char));
  printf("Getting from message box to valid buffer...\n");
  printf("Return code for system call: %d, %s\n\n", errCode, strerror(-errCode));
  free(buffer);
}

void testPutAndGet(void){
  char buffer1[] = "Test 1";
  char buffer2[] = "Test 2";
  char buffer3[] = "Test 3";
  int  length = sizeof(buffer1);

  printf("Putting in message box: \"%s\"\n", buffer1);
  int errCode = syscall(__NR_dm510_msgbox_put, &buffer1, length);
  printf("Return code from system call put: %d\n\n", errCode);
  
  printf("Putting in message box: \"%s\"\n", buffer2);
  errCode = syscall(__NR_dm510_msgbox_put, &buffer2, length);
  printf("Return code from system call put: %d\n\n", errCode);

  printf("Putting in message box: \"%s\"\n", buffer3);
  errCode = syscall(__NR_dm510_msgbox_put, &buffer3, length);
  printf("Return code from system call put: %d\n\n", errCode);

  printf("Getting from message box...\n");
  char* getBuffer1 = malloc(sizeof(length));
  errCode = syscall(__NR_dm510_msgbox_get, getBuffer1, length);
  printf("Result from get system call: \"%s\"\n", getBuffer1);
  printf("Return code of get system call: %d\n\n", errCode);

  printf("Getting from message box...\n");
  char* getBuffer2 = malloc(sizeof(length));
  errCode = syscall(__NR_dm510_msgbox_get, getBuffer2, length);
  printf("Result from get system call: \"%s\"\n", getBuffer2);
  printf("Return code of get system call: %d\n\n", errCode);

  printf("Getting from message box...\n");
  char* getBuffer3 = malloc(sizeof(length));
  errCode = syscall(__NR_dm510_msgbox_get, getBuffer3, length);
  printf("Result from get system call: \"%s\"\n", getBuffer3);
  printf("Return code of get system call: %d\n\n", errCode);
  
  free(getBuffer1);
  free(getBuffer2);
  free(getBuffer3);
}

void testInvalidLengths(){
  char str[] = "Invalid put length";
  printf("Putting in message box: \"%s\", with invalid length...\n", str);
  int errCode = syscall(__NR_dm510_msgbox_put, &str, -1);
  printf("Return code from put system call: %d, %s\n\n", errCode, strerror(-errCode));

  char *buffer = malloc(10);
  printf("Getting from message box with invalid length...\n");
  errCode = syscall(__NR_dm510_msgbox_get, buffer, -1);
  printf("Return code from get system call: %d, %s\n\n", errCode, strerror(-errCode));
  free(buffer);
}

void getWithSmallBuffer(){
  char str[] = "Test put";
  printf("Putting in message box: \"%s\"\n", str);
  int errCode = syscall(__NR_dm510_msgbox_put, &str, sizeof(str));
  printf("Return code of put system call: %d, %s\n\n", errCode, strerror(-errCode));  

  char *buffer = malloc(1);
  printf("Getting from message box with small buffer...\n");
  errCode = syscall(__NR_dm510_msgbox_get, buffer, sizeof(buffer));
  printf("Return code of get system call: %d, %s\n\n", errCode, strerror(-errCode));

  free(buffer);
}

void testWithBadAddress(){
  char str[] = "";
  printf("Putting in message box with bad address and length...\n");
  int errCode = syscall(__NR_dm510_msgbox_put, &str, 10000000000);
  printf("Return code of put system call: %d, %s\n\n", errCode, strerror(-errCode));
}

int main(void){
  printf("TEST IN PROGRESS\n");
  bool run = true;
  while(run){
    printf("--------------------------------------------------\n");
    printf("Enter a number between 1 and 5:\n\n");
    printf("Enter 1 to test get on an empty stack\n");
    printf("Enter 2 test put, get and stack order\n");
    printf("Enter 3 to test invalid lengths in put and get\n");
    printf("Enter 4 to test get with small buffer\n");
    printf("Enter 5 to test put with bad address\n");
    printf("--------------------------------------------------\n");
    int testNumber;
    scanf("%d", &testNumber);
    printf("\n");
    switch (testNumber){
      case 1:
        testEmptyStack();
	break;
      case 2:
	testPutAndGet();
        break;
      case 3:
	testInvalidLengths();
        break;
      case 4:
	getWithSmallBuffer();
	break;
      case 5:
	testWithBadAddress();
	break;
      default:
	printf("Actions not known for entered number, aborting...\n");
	run = false;
    }
  }
  return 0;
}
