#include <stdio.h>
#include <malloc.h>
#include <pthread.h>

#define nb_thread 4

typedef struct{
  int rank;
} openmp_data_t;

#define omp_get_thread_num() data->rank


void *
openmp_parallel (openmp_data_t* data)
{
  int rang;
  float a;
  a = 92290.;

      a = -92290.;
      
  rang = omp_get_thread_num ();
  printf ("Rang : %d ; A vaut : %f\n", rang, a);
}


int
main ()
{
  int i;
  pthread_t *pids;
  openmp_data_t* data;
  float a;
    a = 92290.;
  printf ("Out region: \n");

  pids = malloc (nb_thread * sizeof (pthread_t));
  data= malloc(nb_thread * sizeof (openmp_data_t));

  
  for (i = 0; i < nb_thread; i++)
    {
      data[i].rank=i;
    }
  
  for (i = 1; i < nb_thread; i++)
    {
      pthread_create (&(pids[i]), NULL, (void*(*)(void*))openmp_parallel, (void *) &(data[i]));
    }

  openmp_parallel ((void *) &(data[i]));

  for (i = 1; i < nb_thread; i++)
    {
      pthread_join (pids[i], NULL);
    }
  free (pids);
  free(data);
  return 0;
}

#if 0
int
main ()
{
  int rang;
  float a;

#pragma omp parallel private(a,rang)
  {
    a = 92290.;

#pragma omp single
    {
      a = -92290.;
    }

    rang = omp_get_thread_num ();
    printf ("Rang : %d ; A vaut : %f\n", rang, a);
  }
  return 0;
}
#endif
