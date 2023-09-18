#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


volatile char theChar = '\0';
volatile char afficher = 0;

void *
lire (void *name)
{
  do
    {
      while (afficher == 1)
	usleep (1000000);	/* attendre mon tour */
      theChar = getchar ();
      afficher = 1;		/* donner le tour */
    }
  while (theChar != 'F');
  return NULL;
}

void *
affichage (void *name)
{
  do
    {
      while (afficher == 0)
	usleep (1000000);	/* attendre */
      printf ("car = %c\n", theChar);
      afficher = 0;		/* donner le tour */
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
