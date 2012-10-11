/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

/*
 * Fonctions relatives aux listes chaines.
 */

#include <stdlib.h>

#include "my_functions.h"
#include "listes.h"

cel *creer_liste (void)
{
  return (NULL);
}

cel *creer_cel (void *info)
{
  cel *tmp;

  tmp = my_malloc (sizeof (cel));
  if (!tmp)
    return (NULL); 

  tmp->cel_suiv = NULL;
  tmp->cel_info = info;

  return (tmp);
}

int vider_liste (cel **liste)
{
  cel *tmp ;

  if (!*liste)
    return (1);

  do
  {
    tmp = *liste;
    *liste = tmp->cel_suiv;
    if (tmp->cel_info)
      my_free (tmp->cel_info); 
    my_free (tmp);
  }
  while (*liste);

  return (0);
}

int virer_liste (cel **liste, cel *el)
{
  cel *tmp = *liste;
  cel *avant_tmp = NULL;

  while (tmp != el && tmp)
  {
    avant_tmp = tmp;
    tmp = CEL_NEXT(tmp);
  }

  if (tmp)
  {
    if (avant_tmp) 
      CEL_NEXT(avant_tmp) = CEL_NEXT(CEL_NEXT(avant_tmp));
    else
    {
      *liste = CEL_NEXT(*liste);
      avant_tmp = *liste;
    }

    /* on vire el de la memoire tout ca */
    if (CEL_CONT(el))
      my_free (CEL_CONT(el));
    my_free (el);
    return (0);
  }
  else
    return (-1);

  /* cas improbable */
  return (-1);
}

int ajout_liste (cel **liste, void *info)
{
  cel *tmp;
  cel *new = creer_cel (info);

  if (new == NULL)
    return (-1);

  /* On rajoute 'new' à la fin. */
  new->cel_suiv = NULL;

  if(*liste == NULL) {
    *liste = new;
  } else {
    tmp = *liste;
    while(tmp->cel_suiv != NULL) {
      tmp = tmp->cel_suiv;
    }
    tmp->cel_suiv = new;
  }

  return (0);
}
