#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

volatile char theChar = '\0';
volatile char afficher = 1;
pthread_cond_t cond_lire = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_ecrire = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *
lire (void *name)
{
  do
    {
      while (afficher == 1);
      pthread_mutex_lock (&mutex);
      afficher = 1;
      theChar = getchar ();
      pthread_cond_signal (&cond_ecrire);
      pthread_cond_wait (&cond_lire, &mutex);
      pthread_mutex_unlock (&mutex);
    }
  while (theChar != 'F');
  return NULL;
}

void *
affichage (void *name)
{
  do
    {
      while (afficher == 0);
      pthread_mutex_lock (&mutex);
      afficher = 0;
      pthread_cond_wait (&cond_ecrire, &mutex);
      printf ("car = %c\n", theChar);
      pthread_cond_signal (&cond_lire);
      pthread_mutex_unlock (&mutex);
    }
  while (theChar != 'F');
  return NULL;
}


int
main (void)
{
  pthread_t filsA, filsB;
  if (pthread_create (&filsA, NULL, affichage, "AA"))
    {
      perror ("pthread create");
      exit (EXIT_FAILURE);
    }
  if (pthread_create (&filsB, NULL, lire, "BB"))
    {
      perror ("pthread create");
      exit (EXIT_FAILURE);
    }
  if (pthread_join (filsA, NULL))
    perror ("pthread join");
  if (pthread_join (filsB, NULL))
    perror ("pthread join");
  printf ("Fin du pere\n");
  return (EXIT_SUCCESS);
}
