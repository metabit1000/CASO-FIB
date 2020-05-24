#include <mach.h>
#include <mach_error.h>
#include <mach/mig_errors.h>
#include <mach/thread_status.h>
#include <mach/processor_info.h>
#include <stdio.h>
#include <stdlib.h>
#include <hurd.h>
#include <string.h>

int main(int argc, char *argv[]){
	kern_return_t res;
	mach_port_t child;

	res = task_create(mach_task_self(), 0, &child);
	if (res != KERN_SUCCESS) {
	      printf ("Create error: %s \n", mach_error_string(res));
	      exit(1);
	}

	int size = 16384; //ponemos el size del enunciado
	vm_address_t address;
	res = vm_allocate (child, &address, size, 0);
	if (res != KERN_SUCCESS) {
	      printf ("Allocate error asking for memory: %s \n", mach_error_string(res));
	      exit(1);
	}


	char data[size] __attribute__((aligned(size))); //todo a 0
	res = vm_write (child, address, (vm_offset_t)data, (mach_msg_type_number_t)size);
	if (res != KERN_SUCCESS) {
      		printf ("Write error trying to write: %s \n", mach_error_string(res));
      		exit(1);
	}

	sleep(30);

	res = task_terminate(child);
	if (res != KERN_SUCCESS) {
      		printf ("Terminate error: %s \n", mach_error_string(res));
      		exit(1);
	}

}