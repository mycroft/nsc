/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

/*
 * Header du fichier listes.c
 */

#ifndef _LISTES_HEADER_
#define _LISTES_HEADER_

typedef struct cel_t
{
  struct cel_t *cel_suiv;
  void *cel_info;
} cel, *pcel;

#define CEL_NEXT(tmp) ((tmp)->cel_suiv)
#define CEL_CONT(tmp) ((tmp)->cel_info)

/* Creer un debut de liste vide */
/* aka renvoit NULL */
cel *creer_liste (void);

/* Creer une cellule et remplis les chans */
cel *creer_cel (void *info);

/* Vide une liste tout entiere */
/* Si pour chaque element il existe un chan info,
   il le free(2) */
/* Renvoit 0 si il a efface la liste */
/* Renvoit 1 si la liste etait vide */
int vider_liste (cel **liste);

/* Rajoute a la liste 'liste' l element 'info' */
/* Renvoit 0 si tout est ok */
/* Renvoit -1 si il y a eu un probleme */
int ajout_liste (cel **liste, void *info);

/* Retirer un elemeent de la liste */
int virer_liste (cel **liste, cel *el);

/* Trier une lsite */
cel* trie_liste (cel **via, int (*comparaison)(cel *c1, cel *c2));

#endif
