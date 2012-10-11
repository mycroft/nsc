/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>

#include "my_functions.h"
#include "get_next_line.h"
#include "nsc_user.h"

static char *cequireste = NULL; 

int is_there_get_next_data (void)
{
  if (cequireste)
    return (1);
  else
    return (0);
}

void free_get_next_line_buffer (void)
{
  if (cequireste)
  {
    my_free (cequireste);
    cequireste = NULL;
  }

  return ;
}
unsigned int get_next_20 (char *buf)
{
  int i = 0;
  int l;

  if (!buf)
    return (-2);

  l = my_strlen (buf);

  for (i = 0; i < l; i++)
    if (*(buf + i) == '\n')
      return (i);

  return (-2);
}

char *get_next_line(const int fd)
{
  char *str = NULL;
  char *save;
  char buffer[TAILLE_READ + 1];
  int n;
  int tusors = 0;

  static int r_read = 0; /* len de chars lus */
  static int r_len = 0;  /* nombre de chars dans le buffer */
  int r_ecrit = 0;       /* nombre de chars dans str */

  my_memset (buffer, 0, TAILLE_READ + 1);

  if (r_len)
  {
    my_memcpy (buffer, cequireste, r_len);
    my_free (cequireste);
    cequireste = NULL;
  }

  do
  {
    /* si il ne reste pas des chars dans le 'cequireste' */
    if (!r_len)
    {
      my_memset (buffer, 0, TAILLE_READ + 1);
      r_read = my_read (fd, buffer, TAILLE_READ);
      if (r_read <= 0)
      {
        /* au cas ou il y ait une erreur */ 
        return (str);
      }
      r_len = r_read;
    }

    save = str;
    if (!(str = my_realloc (save, r_ecrit, r_ecrit + TAILLE_READ + 1)))
    {
      str = save;
      return (str);
    }

    n = get_next_20 (buffer);
    n += 1;

    switch (n)
    {
      default:
        /* on a un \n */
        my_memcpy (str + r_ecrit, buffer, n);
        /* -1 ici pour virer le \n a la fin */
        my_memset (str + r_ecrit + n - 1, 0, 1);
        tusors = 1;
        r_len -= n;
        r_ecrit += n;

        if (r_len)
        {
          cequireste = my_malloc (sizeof (char) * (r_len + 1));
          my_memcpy (cequireste, buffer + n, r_len + 1);
        }

      break;
      case -1:
        /* on a pas de \n */

        my_memcpy (str + r_ecrit, buffer, r_len);
        my_memset (str + r_ecrit + r_len, 0, 1);
        r_ecrit += r_len;
        r_len = 0;
      break;
    }
  }
  while (!tusors);

  return (str);
}
