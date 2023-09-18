#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int NB_THREADS;


typedef struct {
  int rank;
} data_thread_t;

data_thread_t *
run (data_thread_t *arg)
{
  int rank;
  rank = arg->rank;
  printf ("Hello, I’am %d (%p) &rank %p & NB_THREADS %p\n", rank,
	  (void *) pthread_self (), &rank, &NB_THREADS);
  return arg;
}

int
main (int argc, char **argv)
{
  pthread_t *pids;
  data_thread_t* data_thread;
  int i;
  NB_THREADS = atoi (argv[1]);
  pids = (pthread_t *) malloc (NB_THREADS * sizeof (pthread_t));
  data_thread = (data_thread_t*)malloc (NB_THREADS * sizeof (data_thread_t));
  
  for (i = 0; i < NB_THREADS; i++)
    {
      data_thread[i].rank = i;
      pthread_create (&(pids[i]), NULL, (void *(*)(void *)) run, (void*) &(data_thread[i]));
    }
  for (i = 0; i < NB_THREADS; i++)
    {
      data_thread_t *res;
      pthread_join (pids[i], (void*)&res);
      fprintf (stderr, "Thread %d Joined\n", res->rank);
      assert (res->rank == i);
    }

  free(pids);
  free(data_thread);
  
  return 0;
}
