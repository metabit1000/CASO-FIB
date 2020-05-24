#include <mach.h>
#include <mach_error.h>
#include <mach/mig_errors.h>
#include <mach/thread_status.h>
#include <mach/processor_info.h>
#include <stdio.h>
#include <stdlib.h>
#include <hurd.h>
#include <string.h>

/* Han sido de ayuda las siguientes webs: 
	https://nshipster.com/inter-process-communication/ 
	https://wiki.darlinghq.org/documentation:mach_ports
*/


int main(int argc, char *argv[]){ 
struct {
    mach_msg_header_t header;
    char text[5]; 
} message_enviado;

struct {
        mach_msg_header_t header;
        char text[5];        
} message_recibido;

	kern_return_t res;
	mach_port_t port;

	res = mach_port_allocate(mach_task_self(),MACH_PORT_RIGHT_RECEIVE,&port);
	if (res != KERN_SUCCESS) {
	      printf ("Allocate error trying to get a port: %s \n", mach_error_string(res));
	      exit(1);
	}
	int pid = fork();
	
	if(pid < 0) { //error
    	printf("Error doing fork()");
   	}
   	else if (pid == 0){ //hijo; el que envia el mensaje
   		message_enviado.header = (mach_msg_header_t) {
	    	.msgh_remote_port = 34,
	    	.msgh_local_port = MACH_PORT_NULL,
	    	.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, 0),
	    	.msgh_size = sizeof(message_enviado)
	    };
	strncpy(message_enviado.text, "Hola", sizeof(message_enviado.text));
	sleep(6);
	mach_msg_return_t error = mach_msg_send(&message_enviado.header); 
      	if (error != MACH_MSG_SUCCESS){
            printf ("Error trying to send the message: %s \n", mach_error_string(error));
	    exit(1);
      	}
      	exit(0);
   	}
   	else { //padre; receptor del mensaje
   		
   	/*
	res = mach_port_insert_right(mach_task_self(), port, port, MACH_MSG_TYPE_MAKE_SEND);
    	if (res != KERN_SUCCESS) {
        	printf ("Insert_right error: %s \n", mach_error_string(res));
        	exit(1);
    	} 	
    	*/

		res = mach_port_insert_right(pid2task(pid), 34, port, MACH_MSG_TYPE_MAKE_SEND);	

		if (res != KERN_SUCCESS) {
	      		printf ("Insert error trying to get a port: %s \n", mach_error_string(res));
	      		exit(1);
		}
   	
   		message_recibido.header = (mach_msg_header_t) {
	    	.msgh_remote_port = MACH_PORT_NULL,
	    	.msgh_local_port = port,
	    	.msgh_bits = MACH_MSGH_BITS( 0, MACH_MSG_TYPE_MOVE_RECEIVE),
	    	.msgh_size = sizeof(message_recibido) };

   	mach_msg_return_t error = mach_msg_receive(&message_recibido.header);
   	if (error != MACH_MSG_SUCCESS){
            printf ("Error trying to receive the message: %s \n", mach_error_string(error));
	    exit(1);
      	}
        sleep(3);
	printf("Obtengo mensaje");
      	int status;
      	//waitpid(-1, &status, 0); //espero que el proceso hijo muera
      	
      	printf("El mensaje enviado es: %s \n",message_recibido.text);
   	}    

}
