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

/*
* Initialisation de la pile de sommet
*/
void initialisation (Pile* pile)
{
  pile->debut = NULL;
  pile->taille = 0;
}

/*
* Empiler un sommet
* Retourne -1 si l'allocation du sommet échoue sinon 0 (tout ce passe bien)
*/
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

/*
* Depiler un sommet
* Retourne -1 si il n'y a aucun sommet dans la pile sinon retourne la valeur du sommet après l'avoir enlevé de la pile
*/
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

/*
* Parcours en profondeur du graphe pour voir si il est connexe
* Retourne 0 si il n'est pas connexe et 1 sinon
*/
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

/* Affiche le littéral l */
void p_litteral (int l)
{
  printf ("%d ", l);
}

/*
 * Convertit le graphe en formule sat pour HAC.
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
  int size = orderG ();                         // Nombre de sommets du graphe

  int i, u, v, w, h, l, nb_clauses;             // Compteurs

  /* Sum 1 -> k -1 */
  int sum = 0;
  int k = profondeur + 1;
  if (k > size)
    {
      fprintf(stderr, "Le graphe ne peut pas avoir d'arbre couvrant de profondeur supérieur a son nombre de sommet: %d", size);
      exit (EXIT_FAILURE);
    }
  for (i = 1; i < k; ++i)
      sum += i;
  printf ("c\nc start with comments\nc\nc\n");    // Commentaires
  nb_clauses =  (size + (pow(k, 2)*size - size*k - sum*size) + (pow(size, 2) - size) + k + (size*(k-1)));
  printf ("p cnf %d %d\n", size*k, nb_clauses);

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

  /* Un des voisins du sommet v est sont parent dans l'arbre couvrant*/
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
}

/*
* Convertit la valuation retourné par le solveur en HAC
* La valuation doit être écrite dans un fichier nommé val situé dans le même répertoire
* Retourne pour chaque sommet leur profondeur dans HAC.
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
    ++k;
    int sommet, profondeur;
    for (int i = 0; i < size; ++i)
      {
	profondeur = tab[i] - h;
	sommet = tab[i]/k;
	if (profondeur == (k-1))
	  --sommet;
        printf ("sommet: %d a la profondeur: %d\n", sommet, profondeur);
        h += k;
      }
    fclose (val);
}

int main(int argc, char* argv [])
{
  /* Pas assez ou trop d'arguments */
  if (argc < 2 || argc > 3)
    {
      fprintf (stderr, "Nombre d'arguments incorrect (tapez ./dm -help).\n");
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
