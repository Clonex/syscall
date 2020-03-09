#include "linux/kernel.h"
#include "linux/unistd.h"
#include <linux/slab.h>
#include <linux/uaccess.h>
#include "linux/errno.h"

typedef struct _msg_t msg_t;

struct _msg_t{
	msg_t* previous;
	int length;
	char* message;
};

static msg_t *bottom = NULL;
static msg_t *top = NULL;

int sys_dm510_msgbox_put( char *buffer, int length ){
	unsigned long flags;
	if( length < 0 ){
		return -EINVAL; // Invalid argument
	}
	if( access_ok(buffer, length) ){
		msg_t* msg = kmalloc(sizeof(msg_t), GFP_KERNEL);

		if( msg != NULL ){
			msg->previous = NULL;
			msg->length = length;
			msg->message = kmalloc(length, GFP_KERNEL);

			if( msg->message == NULL ){
				return -ENOMEM; // Out of memory (the only case kmalloc returns NULL)
			}

			if( copy_from_user( msg->message, buffer, length ) == 0 ){ // Copied everything
				local_irq_save(flags);
				if(bottom == NULL){
					bottom = msg;
					top = msg;
				}
				else {
					/* not empty stack */
					msg->previous = top;
					top = msg;
				}
				local_irq_restore(flags);
				return 0;
			}
			return -EAGAIN; // Try again
		}
		return -ENOMEM; // Out of memory (the only case kmalloc returns NULL)
	}
	return -EFAULT; // Bad address
}

int sys_dm510_msgbox_get( char* buffer, int length ){
	unsigned long flags;
	if( length < 0 ){
		return -EINVAL; // Invalid argument
	}
	local_irq_save(flags);
	if( top == NULL ){
		local_irq_restore(flags);
		return -ENODATA; // No data available
	}
	if( access_ok(buffer, length) ){
		msg_t* msg = top;
		char *tmp = msg->message;
		int mlength = msg->length;

		/* copy message */
		if( copy_to_user( buffer, tmp, mlength ) == 0 ){ //copied everything 
			/* pop the stack */
			top = msg->previous;
			local_irq_restore(flags);

			/* free memory */
			kfree(tmp);
			kfree(msg);

			return mlength;
		}
		local_irq_restore(flags);
		return -EAGAIN; // Try again
	}
	local_irq_restore(flags);
	return -EFAULT; // Bad address
}