#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include <pthread.h>

#define nb_thread 4

typedef struct{
  int rank;
} openmp_data_t;

#define omp_get_thread_num() data->rank


pthread_mutex_t big_lock = PTHREAD_MUTEX_INITIALIZER;

int s, p;

void *
openmp_parallel (openmp_data_t* data)
{
  pthread_mutex_lock (&big_lock);
  {
    s++;
  }
  pthread_mutex_unlock (&big_lock);
  pthread_mutex_lock (&big_lock);
  {
    p *= 2;
  }
  pthread_mutex_unlock (&big_lock);
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

  s = 0, p = 1;
#pragma omp parallel
  {
#pragma omp critical
    {
      s++;
    }
#pragma omp critical
    {
      p *= 2;
    }
  }
  printf ("Somme et produit finaux : %d, %d\n", s, p);
  return 0;
}
#endif
