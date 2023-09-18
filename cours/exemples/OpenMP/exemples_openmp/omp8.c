#include <omp.h>
#include <stdio.h>
#include <pthread.h>

int NB_THREADS = 4;
#define N 5
typedef struct  {
	int rank;
	int *i;
        int *s;
	int *p;
	int *r;
	int loc_red_s;
	int loc_red_r;
	int loc_red_p;
} omp_s_t;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void* main_omp(omp_s_t* args){
	int i; 
	int s;
        int p;
        int r;
	int debut;
	int fin;

	/*Chaque thread récupère une copie locale de chaque variables*/
	s=*(args->s);
	p=*(args->p);
	r=*(args->r);

	fprintf(stderr,"rang %d s %d p %d r %d\n",args->rank,s,p,r);

	/*On calcul les bornes pour chaque thread*/
	debut = (args->rank)*(N/NB_THREADS);
	fin = (args->rank+1)*(N/NB_THREADS);
	if (args->rank < N%NB_THREADS){
		debut += args->rank;
		fin ++;
		fin += args->rank;
	} else {
		debut += N%NB_THREADS;
		fin += N%NB_THREADS;
	}

	fprintf(stderr,"rang %d %d %d\n",args->rank,debut,fin);

	for (i=debut; i<fin; i++) {
		s = s + 1;
		p = p * 2;
		r = r * 3;
	}

	fprintf(stderr,"rang %d debut %d fin %d s %d p %d r %d\n",args->rank,debut,fin,s,p,r);
	
	/*Pour avoir un code juste, il faut ajouter une barrière*/

	/* On stock dans une variable locale pour la future réduction*/
	pthread_mutex_lock(&lock);
	*(args->s) += s;
	*(args->p) *= p;
	*(args->r) *= r;
	pthread_mutex_unlock(&lock);

	return args;
}

void main() {
 int i, s=0, p=1, r=1;
 int i_omp;

 pthread_t pids[NB_THREADS];
 omp_s_t args[NB_THREADS];

 for(i_omp = 0 ; i_omp < NB_THREADS; i_omp++){
	 args[i_omp].rank=i_omp;
	 args[i_omp].i = &i;
	 args[i_omp].s = &s;
	 args[i_omp].p = &p;
	 args[i_omp].r = &r;
	 pthread_create(&pids[i_omp],NULL,(void*(*)(void*))main_omp, &args[i_omp]);
 }
 for(i_omp = 0 ; i_omp < NB_THREADS; i_omp++){
	 omp_s_t * res;
	 pthread_join(pids[i_omp], &res);
 }
#if 0
 for(i_omp = 0 ; i_omp < NB_THREADS; i_omp++){
         omp_s_t * res;
         res = &args[i_omp];
         /*On fait la réduction*/
         s+=res->loc_red_s;
         p*=res->loc_red_p;
         r*=res->loc_red_r;
 }
#endif

 printf("s = %d ; p = %d ; r = %d\n",s,p,r);
 /*
int main()
{
 int i, s=0, p=1, r=1;
 #pragma omp parallel
 {
 #pragma omp for reduction(+:s) reduction(*:p,r)
 for (i=0; i<N; i++) {
 s = s + 1;
 p = p * 2;
 r = r * 3;
 }
 }
 printf("s = %d ; p = %d ; r = %d\n",s,p,r);
 return 0;
}
 */
}
