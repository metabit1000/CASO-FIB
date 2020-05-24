#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

int main(int argc, char * argv []) {
	int res,fd;
	void *p;
	struct timeval tv0, tv1;
	double secs, bw;

	int size = 500 * 1024 * 1024; // 500 Mbytes
	
	if (argc != 2) {
		printf("Falta indicar el fichero\n");
		exit(0);
	}	
	
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

   	//escribir en disco usando fichero
   	FILE *fp = fopen(argv[1], "w");
   	if (fd < 0) {
   		perror("Error opening the file");
   	}
   	
   	res = fseek(fp,size,SEEK_SET);
   	if (res < 0) { 
		perror ("fseek error");
	}

   	fputc('\0', fp); //final del archivo
   	fclose(fp);

	res = gettimeofday(&tv1, NULL);
	if (res < 0) { 
    	perror ("gettimeofday error");
   	}

	secs = (((double)tv1.tv_sec*1000000.0 + (double)tv1.tv_usec) - 
          ((double)tv0.tv_sec*1000000.0 + (double)tv0.tv_usec))/1000000.0;	
   	
   	bw = 500.0/secs;
   	
   	printf (" secs   %.8lf s. \n", secs);
   	printf(" bandwith: %f Mbytes x sec \n", bw);

   	printf("\n");
}
