#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


volatile char theChar = '\0';
volatile int affiche = 0;
void *
lire (void *name)
{
  do
    {
      while (affiche != 0)
	sleep (1);
      theChar = getchar ();
      affiche = 1;
    }
  while (theChar != 'F');
  return NULL;
}

void *
affichage (void *name)
{
  do
    {
      while (affiche == 0)
	sleep (1);
      printf ("car = %c\n", theChar);
      affiche = 0;
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
