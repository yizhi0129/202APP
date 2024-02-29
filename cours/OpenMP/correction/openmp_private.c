#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include <pthread.h>

#define nb_thread 4

typedef struct{
  int rank;
} openmp_data_t;

#define omp_get_thread_num() data->rank

void *
openmp_parallel (openmp_data_t* data)
{
  float a;  
  printf ("In region:%p thread % d \n", &a, omp_get_thread_num ());
  return NULL;
}

int
main ()
{
  int i;
  pthread_t *pids;
  openmp_data_t* data;
  float a;
  a = 92000.;
  printf ("Out region:%p \n", &a);

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
  float a;
  a = 92000.;
  printf ("Out region:%p \n", &a);
#pragma omp parallel private(a)
  {
    printf ("In region:%p thread % d \n", &a, omp_get_thread_num ());
  }
  return 0;
}
#endif
