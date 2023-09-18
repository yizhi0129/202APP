#include <omp.h>
#include <stdio.h>
#include <pthread.h>

int NB_THREADS = 4;

__thread int omp_rank = 0;

typedef struct  {
	int rank;
	float *a;
	float b;
} omp_s_t;

int get_rank(){
	return omp_rank;
}

void sub(float x, float* y){
	*y=x+get_rank();
}

void* main_omp(omp_s_t* args){
	/*Généré par le compilo*/
	float b;
	omp_rank = args->rank;
	b = args->b;

	/*code d'origine*/
	b += 290;
	sub (*(args->a), &b);
	printf("Hello from thread %d %f @a%p @b%p %f\n", get_rank() ,*(args->a), &(args->a) , &(b),b) ;
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
