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
    printf ("In region: thread % d \n", omp_get_thread_num ());
  return NULL;
}

int
main ()
{
  int i;
  pthread_t *pids;
  openmp_data_t* data;
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
  printf ("Out region: \n", );
#pragma omp parallel 
  {
    printf ("In region: thread % d \n", omp_get_thread_num ());
  }
  return 0;
}
#endif
