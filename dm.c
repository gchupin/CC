#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "all.h"

/* Print un littéral */
void p_litteral (int l)
{
  printf ("%d ", l);
}

/*
 * La formule SAT est écrite dans le terminal par défault
 * Il faudra donc la rediriger vers un fichier si besoin.
 */

void hac_to_sat (int k)
{
  int cpt = 0;
  printf ("c\nc start with comments\nc\nc\n");  // Commentaires
  int size = orderG ();                         // Nombre de sommets du graphe
  
  /*Pour l'affichage des littéraux(de la forme nk avec n numéro de sommet et k une profondeur)*/
  //int m = (int)log10(k) + 1;
  //m = pow(10, m);
  
  int i, u, v, w, h, l, nb_clauses;             // Compteurs

  /* Sum 1 -> size -1 */
  int sum = 0;
  for (i = 1; i < size; ++i, sum += i);

  nb_clauses =  (size + (pow(size, 2) - size - sum) + (pow(size, 2) - size) + k + );
  printf ("p cnf %d %d\n", size*k, nb_clauses);         // Littéraux  !TODO
 
  /* Il existe au moins une hauteur par sommet */
  for (v = 0; v < size; ++v) //nb_clause: size
    {
      for (h = 0; h < k; ++h)
	p_litteral (k*v+h+1);
      ++cpt;
      printf ("0\n");                            // Fin de ligne
    }
   printf ("cpt = %d", cpt);
  /* Il existe au plus une hauteur par sommet */
  for (v = 0; v < size; ++v)                   //nb_clause: size² - size - somme(1 à size-1)
    for (h = 0; h < k; ++h)
      for (l = h+1; l < k; ++l)                 //optimisation
	if (l != h)
	  {
	    p_litteral (-(k*v+h+1));
	    p_litteral (-(k*v+l+1));
	    ++cpt;
	    printf ("0\n");                     // Fin de ligne
	  }
   printf ("cpt = %d", cpt);
  /* Il existe un unique sommet v tel que d(v) = 0 */
  for (v = 0; v < size; ++v)
    for (u = 0; u < size; ++u) // nb_clauses : size² - size
      if (u != v)
	{
	  p_litteral (-(v*k+1));
	  p_litteral (-(u*k+1));
	  ++cpt;
	  printf ("0\n");                       // Fin de ligne
	}
   printf ("cpt = %d", cpt);
  /* Il existe au moins un sommet v tel que d(v) = k */
  for (h = 0; h < k; ++h) //nb_clause: k
    {
    for (v = 0; v < size; ++v)
      p_litteral(k*v+h+1);
    ++cpt; 
    printf("0\n");
    }

  /* 1 */
  int print = 0;
  for (v = 0; v < size; ++v)//nb sommets
    for (h = 1; h < k; ++h)
      {
	p_litteral (-(k*v+h+1));
	for (u = 0; u < size; ++u)
	  if (are_adjacent (u, v))
	    {
	      print = 1;
	      
	      p_litteral (k*u+h);
	    }
	if (print)
	  {
	    ++cpt;
	    printf ("0\n");
	    print = 0;
	  }
      }
   printf ("cpt = %d", cpt);
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

int main(int argc, char* argv [])
{
  /* Pas assez ou trop d'arguments */
  if (argc != 2)
    {
      fprintf (stderr, "Il faut 1 unique argument(l'entier k).\n");
      exit (EXIT_FAILURE);
    }
  int k = atoi (argv [1]);
  int n = orderG();
  if (k < n/2 || k > n - 1)
    printf( "Il n'éxiste pas d'arbre couvrant de profondeur %d", k);
  else
    hac_to_sat (atoi (argv [1]));
  return EXIT_SUCCESS;
}
