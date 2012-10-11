/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>

#include "my_functions.h"
#include "nsc_code.h"

int ns_back_slash_decode(char *buff)
{
  char *write;
  char  nbr[3];

  nbr[2] = 0;
  for (write = buff; *buff; buff++, write++)
    if (*buff == BACK_SLASH)
    {
      if (*(buff + 1) == BACK_SLASH)
      {
        *write = BACK_SLASH;
        buff++;
      }
      else if (*(buff + 1) == 'n')
      {
        *write = '\n';
        buff++;
      }
    }
    else
      *write = *buff;
  *write = 0;
  return (1);
}

int ns_url_decode(unsigned char *buff)
{
  char *nwrite;
  char nbr[3];
  int chr;

  nbr[2] = 0;
  for (nwrite = buff; *buff; buff++, nwrite++)
    if (*buff == CHAR_URI_ESC)
    {
      if (*(++buff) == CHAR_URI_ESC)
        *nwrite = CHAR_URI_ESC;
      else
      {
        memcpy(nbr, buff, 2);
        if (sscanf(nbr, "%x", &chr) == 1 && chr < 256)
          *nwrite = (char) chr;
        else
          return (0);
        buff++;
      }
    }
    else
      *nwrite = *buff;
  *nwrite = 0;
  return (1);
}

char *ns_url_encode(unsigned char *buff)
{
  int nbr;
  char *ret;

  for (ret = buff, nbr = 0; *ret; ret++)
    if (!((*ret >= 'A' && *ret <= 'Z') || (*ret >= 'a' && *ret <= 'z') ||
          (*ret >= '0' && *ret <= '9')))
      nbr += 3;
    else
      nbr++;

  ret = my_malloc (nbr + 2);
  my_memset (ret, 0, nbr + 2);

  for (nbr = 0; *buff; buff++)
    if (!((*buff >= 'A' && *buff <= 'Z') || (*buff >= 'a' && *buff <= 'z') ||
          (*buff >= '0' && *buff <= '9')))
    {
      sprintf(ret + nbr, "%%%02X", *buff);
      nbr += 3;
    }
    else
    {
      ret[nbr] = *buff;
      nbr++;
    }
  return (ret);
}

char *ns_back_slash_encode(char *buff)
{
  int  nbr;
  char *ret;

  for (ret = buff, nbr = 0; *ret; ret++)
    if (*ret == BACK_SLASH_N)
      nbr += 2;
    else
      nbr++;

  ret = my_malloc (nbr + 2);
  my_memset (ret, 0, nbr + 2);

  for (nbr = 0; *buff; buff++)
    if (*buff == BACK_SLASH_N)
    {
      sprintf(ret + nbr, "\\n");
      nbr += 2;
    }
    else
    {
      ret[nbr] = *buff;
      nbr++;
    }
  return (ret);
}

char *ns_encode(char *buff)
{
  char *tmp;
  char *ret;

  if ((tmp = ns_back_slash_encode(buff)) == NULL)
    return (NULL);
  if ((ret = ns_url_encode(tmp)) == NULL)
  {
    my_free (tmp);
    return (NULL);
  }
  my_free (tmp);
  return (ret);
}

char *ns_decode(char *buff)
{
  char *ret;

  ret = my_malloc_strcpy (buff);
  if (ns_url_decode(ret) == 0)
  {
    my_free(ret);
    return (NULL);
  }
  if (ns_back_slash_decode(ret) == 0)
  {
    my_free(ret);
    return (NULL);
  }
  return (ret);
}
