#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 

int r=1;
pthread_mutex_t mutex;

void* f(void *x) {
	pthread_mutex_lock(&mutex); //utilitza mprotect(PROT_READ|WRITE)
	r = r * *(int *)x;
	pthread_mutex_unlock(&mutex); //utilitza mprotect(PROT_NONE)
	pthread_exit(NULL);
}

int main(int argc, char** argv) {
	int NTHREADS;
	int *n;
	int i;
	pthread_t *pth;
	pthread_attr_t attr;

	if (argc != 2) {
		printf("Falta indicar el numero de threads\n");
		exit(0);
	}
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&mutex,NULL); //utiliza futex()
	NTHREADS = atoi(argv[1]);

	n = malloc(NTHREADS*sizeof(int));
	pth = (pthread_t*)malloc(NTHREADS*sizeof(pthread_t));

	if (n == NULL || pth == NULL) {
		printf("error reservando memoria");
	}

	for (i=0;i<NTHREADS;i++)
		n[i]=i+1;
	
	for (i=0;i<NTHREADS;i++) 
		pthread_create(&pth[i], &attr, f, (void*)&n[i]); //utiliza clone()

	for(i=0;i<NTHREADS;i++) 
		pthread_join(pth[i], NULL);
	
	printf("factorial de %d = %d\n",NTHREADS,r);
	return 0;
}
