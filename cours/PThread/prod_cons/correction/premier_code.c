#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int NB_THREADS;

void *
run (void *arg)
{
  long rank;
  rank = (long) arg;
  printf ("Hello, I’am %ld (%p) &rank %p & NB_THREADS %p\n", rank,
	  (void *) pthread_self (), &rank, &NB_THREADS);
  return arg;
}

int
main (int argc, char **argv)
{
  pthread_t *pids;
  long i;
  NB_THREADS = atoi (argv[1]);
  pids = (pthread_t *) malloc (NB_THREADS * sizeof (pthread_t));
  for (i = 0; i < NB_THREADS; i++)
    {
      pthread_create (&(pids[i]), NULL, (void *(*)(void *)) run, (void *) i);
    }
  for (i = 0; i < NB_THREADS; i++)
    {
      void *res;
      pthread_join (pids[i], &res);
      fprintf (stderr, "Thread %ld Joined\n", (long) res);
      assert (res == (void *) i);
    }
  return 0;
}
