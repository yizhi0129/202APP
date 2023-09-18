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

void* main_omp_s(omp_s_t* args){
        int i;
        int s;
        int debut;
        int fin;

        /*Chaque thread récupère une copie locale de chaque variables*/
        s=*(args->s);

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

        for (i=debut; i<fin; i++) {
                s = s + 1;
        }

        /* On stock dans une variable locale pour la future réduction*/
        args->loc_red_s = s;

        return args;
}
void* main_omp_p(omp_s_t* args){
        int i;
        int p;
        int debut;
        int fin;

        /*Chaque thread récupère une copie locale de chaque variables*/
        p=*(args->p);

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

        for (i=debut; i<fin; i++) {
                p = p * 2;
        }


        /* On stock dans une variable locale pour la future réduction*/
        args->loc_red_p = p;

        return args;
}

void* main_omp_r(omp_s_t* args){
	int i; 
        int r;
	int debut;
	int fin;

	/*Chaque thread récupère une copie locale de chaque variables*/
	r=*(args->r);

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

        for (i=debut; i<fin; i++) {
		r = r * 3;
	}

	
	/* On stock dans une variable locale pour la future réduction*/
	args->loc_red_r = r;

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
         pthread_create(&pids[i_omp],NULL,(void*(*)(void*))main_omp_s, &args[i_omp]);
 }
 for(i_omp = 0 ; i_omp < NB_THREADS; i_omp++){
         omp_s_t * res;
         pthread_join(pids[i_omp], &res);
 }
 for(i_omp = 0 ; i_omp < NB_THREADS; i_omp++){
         omp_s_t * res;
         res = &args[i_omp];
         /*On fait la réduction*/
         s+=res->loc_red_s;
 }

  for(i_omp = 0 ; i_omp < NB_THREADS; i_omp++){
         args[i_omp].rank=i_omp;
         args[i_omp].i = &i;
         args[i_omp].p = &p;
         pthread_create(&pids[i_omp],NULL,(void*(*)(void*))main_omp_p, &args[i_omp]);
 }
 for(i_omp = 0 ; i_omp < NB_THREADS; i_omp++){
         omp_s_t * res;
         pthread_join(pids[i_omp], &res);
 }
 for(i_omp = 0 ; i_omp < NB_THREADS; i_omp++){
         omp_s_t * res;
         res = &args[i_omp];
         /*On fait la réduction*/
         p*=res->loc_red_p;
 }
 
  for(i_omp = 0 ; i_omp < NB_THREADS; i_omp++){
         args[i_omp].rank=i_omp;
         args[i_omp].i = &i;
         args[i_omp].r = &r;
         pthread_create(&pids[i_omp],NULL,(void*(*)(void*))main_omp_r, &args[i_omp]);
 }
 for(i_omp = 0 ; i_omp < NB_THREADS; i_omp++){
         omp_s_t * res;
         pthread_join(pids[i_omp], &res);
 }
 for(i_omp = 0 ; i_omp < NB_THREADS; i_omp++){
         omp_s_t * res;
         res = &args[i_omp];
         /*On fait la réduction*/
         r*=res->loc_red_r;
 }



 printf("s = %d ; p = %d ; r = %d\n",s,p,r);
 /*
int main()
{
 int i, s=0, p=1, r=1;
 {
 #pragma omp parallel for reduction(+:s) 
 for (i=0; i<N; i++) {
 s = s + 1;
 }
#pragma omp parallel for reduction(*:p) 
 for (i=0; i<N; i++) {
 p = p * 2;
 }
#pragma omp parallel for reduction(*:r) 
 for (i=0; i<N; i++) {
 r = r * 3;
 }
 }
 printf("s = %d ; p = %d ; r = %d\n",s,p,r);
 return 0;
}
 */
}
