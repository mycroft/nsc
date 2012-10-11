/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

/* Contact list functions */
#include <stdio.h>
#include <stdlib.h>
#include "my_functions.h"
#include "nsc_functions.h"
#include "nsc_contact.h"
#include "listes.h"

static pcel contact_list = NULL;
static pcel ignore_list = NULL;
/* static pcel online_list = NULL; */

#define CCL_CONT(x) ((struct contact_list_t*)CEL_CONT(x))
#define OCL_CONT(x) ((struct online_list_t*)CEL_CONT(x))

int add_contact (char *name)
{
  struct contact_list_t *nouvelle;

  if (name == NULL)
    return (-1);
  if (contact_list == NULL)
    contact_list = creer_liste();
  else {
    if(search_first_cel_with_name_in_list(name, contact_list) != NULL)
      return -1;
  }

  nouvelle = my_malloc (sizeof (struct contact_list_t));
  nouvelle->name = my_malloc_strcpy (name);
  ajout_liste (&contact_list, nouvelle);
  
  return 0;
}

int add_ignore (char *name)
{
  char *realname;
  char *message;
  struct ignore_list_t *nouvelle;

  if (name == NULL)
    return (-1);
  if (ignore_list == NULL)
    ignore_list = creer_liste();
  else {
    if(search_first_cel_with_name_in_list(name, ignore_list) != NULL)
      return -1;
  }

  realname = str_to_first_word(name);
  message = name + my_strlen (realname);
  message = strip_spaces(message);

  nouvelle = my_malloc (sizeof (struct ignore_list_t));
  nouvelle->name = my_malloc_strcpy(realname);
  if (message != NULL && *message != '\0')
    nouvelle->message = my_malloc_strcpy(message);
  else
    nouvelle->message = NULL;
  ajout_liste (&ignore_list, nouvelle);

  free(realname);
  if (message != NULL)
    free(message);
  return 0;
}

int rem_contact(char *name)
{
  cel *t;
#define C_NAME(x) ((struct contact_list_t*)CEL_CONT(x))->name
  do {
    t = search_first_cel_with_name_in_list(name, contact_list);
    if (t) {
      if (C_NAME(t) != NULL)
        free(C_NAME(t));
      virer_liste(&contact_list, t);
    }
  } while (t != NULL);

  return 0;
}

int rem_ignore(char *name)
{
  cel *t;
#define I_NAME(x) ((struct ignore_list_t*)CEL_CONT(x))->name
#define I_MSG(x)  ((struct ignore_list_t*)CEL_CONT(x))->message
  do {
    t = search_first_cel_with_name_in_list(name, ignore_list); 
    if (t) {
      if (I_NAME(t) != NULL)
        free(I_NAME(t));
      if (I_MSG(t) != NULL)
        free(I_MSG(t));
      virer_liste(&ignore_list, t);
    }
  } while (t != NULL);
  return 0;
}

int flush_list(pcel liste)
{
  pcel tempcel;
  pcel nextcel;

  tempcel = liste;
  while (tempcel)
  {
    nextcel = CEL_NEXT(tempcel);
    if (CCL_CONT(tempcel)->name)
      my_free (CCL_CONT(tempcel)->name);

    virer_liste (&liste, tempcel);
    tempcel = nextcel; 
  }

  return 0;
}

int flush_contact_list(void)
{
  int t = flush_list(contact_list);
  contact_list = NULL;
  return (t);
}

int flush_ignore_list(void)
{
  int t = flush_list(ignore_list);
  ignore_list = NULL;
  return (t);
}

cel *search_first_cel_with_name_in_list(char *name, pcel liste)
{
  pcel tempcel;

  tempcel = liste;
  while (tempcel)
  {
    if (my_strcmp (CCL_CONT(tempcel)->name, name) == 0)
      return (tempcel);

    tempcel = CEL_NEXT(tempcel);
  }

  return (NULL);
}

struct ignore_list_t *is_ignore(char *name)
{
  pcel t;

  if (name == NULL)
    return (NULL);

  if ((t = search_first_cel_with_name_in_list(name, ignore_list)))
    return (CEL_CONT(t));
  else
    return (NULL);
}

char *get_next_contact_list_name (void)
{
  static pcel temp_char = NULL;

  if (!temp_char)
    temp_char = contact_list;
  else
    temp_char = CEL_NEXT(temp_char);

  if (temp_char)
    return (CCL_CONT(temp_char)->name);
  else
    return (NULL);
}

struct ignore_list_t *get_next_ignore_list_name(void)
{
  static pcel temp_char = NULL;

  if (!temp_char)
    temp_char = ignore_list;
  else
    temp_char = CEL_NEXT(temp_char);

  if (temp_char)
    return ((struct ignore_list_t*)CCL_CONT(temp_char));
  else
    return (NULL);
}

void list_contact_list (void)
{
  char *t;
  while ((t = get_next_contact_list_name ()))
  {
    printf ("[%s]\n", t);
  }
  return ;
}

void list_ignore_list(void)
{
  struct ignore_list_t *t;
  while ((t = get_next_ignore_list_name ()))
  {
    printf ("[%s] %s\n", t->name, t->message);
  }
  return ;
}

int num_contact_list(void)
{
  int n = 0;
  char *t;

  while ((t = get_next_contact_list_name ()))
    n ++;

  return (n);
}

char *build_accolades_contact_list (void)
{
  char *str;
  char *t;
  int len;

  /* De la liste des contacts, je cree ca :
        {nick1,nick2,nick3,etc...}\0
   */

  str = my_malloc (sizeof (char) * 2);
  my_strcpy (str, "{");

  while ((t = get_next_contact_list_name ()))
  {
    len = my_strlen (str);
    str = my_realloc (str, len, (len + my_strlen (t) + 3) * sizeof (char));

    if (len != 1)
    {
      my_strcpy (str + len, ",");
      my_strcpy (str + len + 1, t); /*, my_strlen (t)); */
    }
    else
      my_strcpy (str + 1, t);
  }

  my_strcat (str, "}");

  return (str);
}
