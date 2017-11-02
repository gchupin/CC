#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "all.h"

typedef struct ListSommet
{
    int sommet;
    struct ListSommet* suivant;
}Sommet;

typedef struct PileSommet
{
  Sommet* debut;
  int taille;
}Pile;

void initialisation (Pile* pile)
{
  pile->debut = NULL;
  pile->taille = 0;
}

int empiler (Pile* pile, int sommet)
{
  Sommet* nouveau_sommet;
  if ((nouveau_sommet = (Sommet *) malloc (sizeof (Sommet))) == NULL)
    return -1;
  nouveau_sommet->sommet = sommet;
  nouveau_sommet->suivant = pile->debut;
  pile->debut = nouveau_sommet;
  ++pile->taille;
  return 0;
}

int depiler (Pile* pile)
{
  if (pile->taille == 0)
    return -1;
  Sommet* supp_sommet = pile->debut;
  int ret = supp_sommet->sommet;
  pile->debut = pile->debut->suivant;
  free (supp_sommet);
  --pile->taille;
  return ret;
}

int connexe ()
{
  Pile* pile = (Pile*) malloc(sizeof (Pile));
  initialisation (pile);
  int size = orderG ();
  int marque[size];
  for (int i = 0; i < size; ++i)
    marque[i] = 0;
  marque[0] = 1;
  empiler (pile, 0);
  int s;
  while ((s = depiler (pile)) != -1)
  {
    for (int i = 0; i < size; ++i)
      if (are_adjacent (s, i))
        if (marque [i] == 0)
          {
            empiler (pile, i);
            marque [i] = 1;
          }
  }
  free (pile);
  for (int i = 0; i < size; ++i)
    if (marque [i] == 0)
      return 0;
  return 1;
}
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
void graphe_to_sat (int profondeur)
{
  if (connexe() == 0)
  {
    fprintf(stderr, "Le graphe n'est pas connexe donc ne posséde aucun arbre couvrant\n");
    exit (EXIT_FAILURE);
  }
  printf ("c\nc start with comments\nc\nc\n");  // Commentaires
  int size = orderG ();                         // Nombre de sommets du graphe

  /*Pour l'affichage des littéraux(de la forme nk avec n numéro de sommet et k une profondeur)*/
  //int m = (int)log10(k) + 1;
  //m = pow(10, m);

  int i, u, v, w, h, l, nb_clauses;             // Compteurs

  /* Sum 1 -> size -1 */
  int sum = 0;
  int k = profondeur + 1;
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
	if (print)
	  {
	    printf ("0\n");
	    print = 0;
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
void val_to_hac (int k)
{
  FILE* val = NULL;
  val = fopen ("val", "r");
  if (val == NULL)
  {
    fprintf (stderr, "Impossible d'ouvrir le fichier val\n");
    exit(EXIT_FAILURE);
  }
  int cpt = 0;
  int taille = 1024;
  int size = orderG();
  int tab[size];
  int caractereActuel;
  while (1)
    {
      int ret = fscanf (val, "%d", &caractereActuel);
      if (ret == EOF)
        break;
      if (caractereActuel > 0)
        {
          tab [cpt] = caractereActuel;
          ++cpt;
        }
    }
    int h = 1;
  //  printf("sommet: %d a la hauteur: %d\n",  tab[i]/k, tab[i]);
    ++k;
    for (int i = 0; i < size; ++i)
      {
        printf ("sommet: %d a la profondeur: %d\n", tab[i]/k, tab[i]-h);
        h += k;
      }
    fclose (val);
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
      if (strcmp (argv[1], "help") == 0)
        printf("Utilisez './dm k' pour convertir le graphe en formule sat de HAC de hauteur k\nSinon utilisez './dm v k' pour avoir HAC en fonction du retour du solveur(v) contenu dans le fichier val et de la hauteur(k)");
      else
        graphe_to_sat (atoi (argv [1]));
  }
  else if (argc == 3)
  {
    if (strcmp (argv[1], "v") == 0)
      val_to_hac (atoi (argv[2]));
  }
  return EXIT_SUCCESS;
}
