#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "all.h"

/* Print un littéral */
void p_litteral (int l)
{
  printf ("%d ", l);
}

/*
 * Convertit le graphe en formule sat pour HAC
 * La formule SAT est écrite dans le terminal par défault
 * Il faudra donc la rediriger vers un fichier si besoin.
 */
void graphe_to_sat (int k)
{
  printf ("c\nc start with comments\nc\nc\n");  // Commentaires
  int size = orderG ();                         // Nombre de sommets du graphe

  /*Pour l'affichage des littéraux(de la forme nk avec n numéro de sommet et k une profondeur)*/
  //int m = (int)log10(k) + 1;
  //m = pow(10, m);

  int i, u, v, w, h, l, nb_clauses;             // Compteurs

  /* Sum 1 -> size -1 */
  int sum = 0;
  for (i = 1; i < k; ++i)
      sum += i;

  nb_clauses =  (size + (pow(k, 2)*size - size*k - sum*size) + (pow(size, 2) - size) + k + (size*(k-1)));
  printf ("p cnf %d %d\n", size*k, nb_clauses);         // Littéraux  !TODO

  /* Il existe au moins une hauteur par sommet */
  for (v = 0; v < size; ++v) //nb_clause: size
    {
      for (h = 0; h < k; ++h)
	p_litteral (k*v+h+1);
      printf ("0\n");                            // Fin de ligne
    }
  /* Il existe au plus une hauteur par sommet */
  for (v = 0; v < size; ++v)                   //nb_clause: size*k² - size*k - somme(1 à k-1)*size
    for (h = 0; h < k; ++h)
      for (l = h+1; l < k; ++l)                 //optimisation
	if (l != h)
	  {
	    p_litteral (-(k*v+h+1));
	    p_litteral (-(k*v+l+1));
	    printf ("0\n");                     // Fin de ligne
	  }
  /* Il existe un unique sommet v tel que d(v) = 0 */
  for (v = 0; v < size; ++v)
    for (u = 0; u < size; ++u) // nb_clauses : size² - size
      if (u != v)
	{
	  p_litteral (-(v*k+1));
	  p_litteral (-(u*k+1));
	  printf ("0\n");                       // Fin de ligne
	}
  /* Il existe au moins un sommet v tel que d(v) = k */
  for (h = 0; h < k; ++h) //nb_clause: k
    {
    for (v = 0; v < size; ++v)
      p_litteral(k*v+h+1);
    printf("0\n");
    }

  /* 1 */
  int print = 0;
  int nb_nonVoisin = 0;
  for (v = 0; v < size; ++v)
    for (h = 1; h < k; ++h)//nb_clause: size*(k-1)
      {
	p_litteral (-(k*v+h+1));
	for (u = 0; u < size; ++u)
	  if (are_adjacent (u, v))
	    {
	      print = 1;

	      p_litteral (k*u+h);
	    }
      else
        ++ nb_nonVoisin;
	if (print)
	  {
	    printf ("0\n");
	    print = 0;
	  }
    //Si le sommet n'a pas de voisin alors unsat
    if (nb_nonVoisin == size)
    {
        nb_nonVoisin = 0;
        p_litteral (k*v+h+1);
        printf ("0\n");
    }
  }

  /* 4
  for (v = 0; v < size; ++v)//nb sommets
    for (u = 0; u < size; ++u)
      for (w = 0; w < size; ++w)
	{
	  if (w != u && u != v)
	    if (are_adjacent (v, u) && are_adjacent (v, w))
	      {
		for (h = 1; h < k; ++h)
		  {
		    p_litteral (-(k*v+h+1));
		    p_litteral (-(k*u+(h)));
		    p_litteral (-(k*w+(h)));
		    ++cpt;
		    printf ("0\n");
		  }
	      }
	}*/

}

/*
* Convertit la valuation retourné par le solveur en HAC
* La valuation doit être écrite dans un fichier nommé val
*/
void val_to_hac ()
{
  FILE* val = NULL;
  val = fopen ("val", "r");
  if (val == NULL)
  {
    fprintf (stderr, "Impossible d'ouvrir le fichier val\n");
    exit(EXIT_FAILURE);
  }
  printf ("test\n");
  int cpt;
  int size = orderG();
  int tab[size];
  int caractereActuel;
  do {
      caractereActuel = fgetc (val);
      if (caractereActuel != ' ' && caractereActuel != EOF)
        {
          tab[cpt] = caractereActuel;
          ++cpt;
        }
  } while(caractereActuel != EOF);
  for (int i = 0; i < size; ++i)
  {
    printf ("%d\n", tab[i]);
  }
}

int main(int argc, char* argv [])
{
  /* Pas assez ou trop d'arguments */
  if (argc < 2)
    {
      fprintf (stderr, "Pas assez d'arguments.\n");
      exit (EXIT_FAILURE);
    }
  else if (argc == 2)
  {
    if (strcmp (argv[1], "val") == 0)
      val_to_hac ();
    else
      graphe_to_sat (atoi (argv [1]));
  }
  return EXIT_SUCCESS;
}
