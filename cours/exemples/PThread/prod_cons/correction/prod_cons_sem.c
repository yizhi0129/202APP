#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

volatile char theChar = '\0';
sem_t can_print;
sem_t can_read;

void *
lire (void *name)
{
  do
    {
      sem_wait (&can_read);
      theChar = getchar ();
      sem_post (&can_print);
    }
  while (theChar != 'F');
  return NULL;
}

void *
affichage (void *name)
{
  do
    {
      sem_wait (&can_print);
      printf ("car = %c\n", theChar);
      sem_post (&can_read);
    }
  while (theChar != 'F');
  return NULL;
}


int
main (void)
{
  pthread_t filsA, filsB;
  sem_init (&can_read, 0, 1);
  sem_init (&can_print, 0, 0);

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

  sem_destroy (&can_read);
  sem_destroy (&can_print);

  printf ("Fin du pere\n");
  return (EXIT_SUCCESS);
}
