#include <mach.h>
#include <mach_error.h>
#include <mach/mig_errors.h>
#include <mach/thread_status.h>
#include <mach/processor_info.h>
#include <stdio.h>
#include <stdlib.h>
#include <hurd.h>
#include <string.h>
//#include <mach/machine/thread_status.h>

void func(){
	printf("entra");
	while(1){}
}

int main(int argc, char *argv[]){
	kern_return_t res;
	mach_port_t thid;

	//printf ("e1");	
	res = thread_create (mach_task_self(), &thid);
	if (res != KERN_SUCCESS) {
	      printf ("Create error: %s \n", mach_error_string(res));
	      exit(1);
	}
	//printf("e2");
	
	struct i386_thread_state state;
	mach_msg_type_number_t state_count = THREAD_STATE_MAX;
	res = thread_get_state(thid,i386_THREAD_STATE, (thread_state_t)&state,&state_count);
	if (res != KERN_SUCCESS) {
	      printf ("Get_state error: %s \n", mach_error_string(res));
	      exit(1);
	}
	
	int stack_size = 300;	//como ejemplo
	state.eip = &func;
	state.uesp = malloc(stack_size) + stack_size;
	
	res = thread_set_state(thid, i386_THREAD_STATE, (thread_state_t)&state,&state_count);
	if (res != KERN_SUCCESS) {
	      printf ("Set_state error: %s \n", mach_error_string(res));
	      exit(1);
	}
	
	res = thread_resume(thid);
	if (res != KERN_SUCCESS) {
	      printf ("Resume error: %s \n", mach_error_string(res));
	      exit(1);
	}
	
	printf ("New thread id %d\n", thid);
	

	//sleep(30);	
	//thread_terminate(thid);
}

