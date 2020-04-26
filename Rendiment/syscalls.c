#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define N 1000000  //numero de veces que se ejecutara la syscall
#define N2 10000   //para fork()


int main(int argc, char * argv []) {
	int res, pid, i;
	void *p;
	int * wstatus;
	struct timeval tv0, tv1;
	double secs;
	int syscall;

	if (argc != 2) {
		printf("Falta indicar la syscall correspondiente. [0...4]: sbrk(0), sbrk(inc), sched_yield(), getpid(), fork/waitpid\n");
		exit(0);
	}	

	syscall = atoi(argv[1]);

	/* Fake call to warm-up */
	res = gettimeofday(&tv0, NULL);
	if (res < 0) { 
		perror ("gettimeofday error");
	}
	/* Fake call to warm-up */
	usleep(100);

	res = gettimeofday(&tv0, NULL);
	if (res < 0) { 
		perror ("gettimeofday error");
	}

   	if (syscall == 0) { //sbrk(0)
   		for (i = 0; i < N; ++i) {
		   	p = sbrk(0);
	   	}
   	}
   	else if (syscall == 1) { //sbrk(inc)
   		void * aux = sbrk(0); 
   		for (i = 0; i < N; ++i) {
		   	p = sbrk(aux);
		   	aux += 256;
	   	}
   	}
   	else if (syscall == 2) { //sched_yield()
   		for (i = 0; i < N; ++i) {
   			res  = sched_yield();
		   	if (res<0){
		   		printf("sched_yield error\n");
				exit(1);
		   	}
	   	}
   	}
   	else if (syscall == 3) { //getpid()
   		for (i = 0; i < N; ++i) {
   			pid  = getpid();
			if (pid < 0){
				printf("getpid error\n");
				exit(1);
			}
	   	}
   	}
   	else if (syscall == 4) { //fork/waitpid
   		for (i = 0; i < N2; ++i) {
   			pid = fork();
   			if (pid == 0) {
   				//printf("Hola, soy el hijo");
   				exit(1); //finaliza el hijo
   			}

   			if (pid < 0){
   				printf("Eror al crear el hijo con fork()\n");
				exit(1);
   			}

   			while (waitpid(-1,wstatus, 0) <= 0); //el padre espera que termine el hijo
	   	}
   	}
   	
   	
   	res = gettimeofday(&tv1, NULL);
	if (res < 0) { 
		perror ("gettimeofday error");
	}
	
	secs = (((double)tv1.tv_sec*1000000.0 + (double)tv1.tv_usec) - 
	       ((double)tv0.tv_sec*1000000.0 + (double)tv0.tv_usec))/1000000.0;	
   	
   	printf (" Tiempo de ejecucion:   %.8lf usecs. \n", secs*1000000.0); //pasamos a microsegundos ya que el enunciado lo pide asi
   	printf("\n");
}