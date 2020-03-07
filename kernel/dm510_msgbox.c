#include "dm510_msgbox.h"
#include <stdlib.h>
#include <string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

typedef struct _msg_t msg_t;

struct _msg_t{
	msg_t* previous;
	int length;
	char* message;
};

static msg_t *bottom = NULL;
static msg_t *top = NULL;

int dm510_msgbox_put( char *buffer, int length ){
	if( length < 0 ){
		return EINVAL; //Invalid argument
	}
	else if( access_ok(buffer, length) ){
		msg_t* msg = kmalloc(sizeof(msg_t), GFP_KERNEL);

		if( msg != NULL ){
			msg->previous = NULL;
			msg->length = length;
			msg->message = kmalloc(length, GFP_KERNEL);

			if( copy_from_user( msg->message, buffer, length ) == 0 ){ //copied everything
				if(bottom == NULL) {
					bottom = msg;
					top = msg;
				}
				else {
					/* not empty stack */
					msg->previous = top;
					top = msg;
				}
				return 0;
			}

		}

	}
	return -14; //Bad address
}

int dm510_msgbox_get( char* buffer, int length ) {
	if( length < 0 ){
		return -22; //Invalid argument
	}
	else if( access_ok(buffer, length) && top != NULL ){
		msg_t* msg = top;
		char *tmp = msg->message;
		int mlength = msg->length;

		/* copy message */
		if( copy_to_user( buffer, tmp, mlength ) == 0 ){ //copied everything 
			/* pop the stack */
			top = msg->previous;

			/* free memory */
			kfree(tmp, GFP_KERNEL);
			kfree(msg, GFP_KERNEL);

			return mlength;
		}
		return 100; //TODO: Find accurate exit code
	}
	return -14; //Bad address
}

int main ( void ) {
	/* test code here */
	return 0;
}