#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include <pthread.h>
#include <math.h>

#define min(a,b) (((a)<(b))?(a):(b))

#define nb_thread 4

typedef struct{
  int rank;
} openmp_data_t;

#define omp_get_thread_num() data->rank


void *
openmp_parallel (openmp_data_t* data)
{
  int i;
  for (i = ; i < ; i++)
    {
      printf ("Thread %d running iteration %d\n", omp_get_thread_num (), i);
    }
}

int
main ()
{
  int i;
  int N;

  N = 10;
  pthread_t *pids;
  openmp_data_t* data;

  pids = malloc (nb_thread * sizeof (pthread_t));
  data = malloc (nb_thread * sizeof (openmp_data_t));
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
main (int argc, char **argv)
{
  int N;

  N = 10;

#pragma omp parallel
  {
    int i;
#pragma omp for schedule(static)
    for (i = 0; i < N; i++)
      {
	printf ("Thread %d running iteration %d\n", omp_get_thread_num (), i);
      }
  }
  return 0;
}
#endif
