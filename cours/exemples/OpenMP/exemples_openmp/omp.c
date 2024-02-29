#include </usr/local/Cellar/libomp/17.0.6/include/omp.h>
#include <stdio.h>
#include <pthread.h>

int NB_THREADS = 4;

typedef struct  {
	int rank;
	float *a;
	float b;
} omp_s_t;

void* main_omp(omp_s_t* args){
	float b;
	b = args->b;

	b += 290;
	printf("Hello from thread %d %f @a%p @b%p %f\n", args->rank ,*(args->a), &(args->a) , &(b),b) ;
	return NULL;
}

void main() {
 float a;
 float b;

 a = 92290.;
 b = 92000.;

 int i; 

 pthread_t pids[NB_THREADS];
 omp_s_t args[NB_THREADS];

 for(i = 0 ; i < NB_THREADS; i++){
	 args[i].rank=i;
	 args[i].a = &a;
	 args[i].b = b;
	 pthread_create(&pids[i],NULL,(void*(*)(void*))main_omp, &args[i]);
 }
 for(i = 0 ; i < NB_THREADS; i++){
	 pthread_join(pids[i], NULL);
 }

 printf("Outside %f @a%p @b%p %f\n",a ,&a, &b, b) ;

 /*
#pragma omp parallel firstprivate(b)
 {
 b += 290;
 printf("Hello from thread %d %f @a%p @b%p %f\n", omp_get_thread_num(),a ,&a, &b, b) ;
 }
 */
}
