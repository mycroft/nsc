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
#include <stdarg.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include <string.h>

#include "my_functions.h"
#include "nsc_user.h"

static unsigned int malloc_cpt = 0;
static unsigned int free_cpt = 0;


#ifdef DEBUG
void *my_function_malloc (size_t size, char *s, char *f, int i)
#else
void *my_malloc (size_t size)
#endif
{
  void *buf;

  if ((buf = malloc ( size )))
    malloc_cpt ++ ;
  else
  {
    printf_cl("malloc: %s\n", strerror(errno));
    exit (EXIT_FAILURE);
  }

  return ( buf );
}

#ifdef DEBUG
void my_function_free (void *buf, char *s, char *f, int i)
#else
void my_free (void *buf)
#endif
{
  if (buf)
  {
    free_cpt ++ ;
    free ( buf );
  }

  return ;
}

void my_memory_repport (void)
{
  fprintf (stderr, "Memory alloc: %i\nMemory freed: %i\n", 
    malloc_cpt, free_cpt);
  return ;
}

void *my_realloc (void *ptr, size_t oldsize, size_t newsize)
{
  void *buf;

  buf = my_malloc (newsize);
  if (ptr)
    my_memcpy (buf, ptr, (oldsize>newsize)?newsize:oldsize);
  my_free (ptr);

  return (buf);
}

void *my_memcpy(void *dst, const void *src, size_t len)
{
  size_t i;
  for (i = 0; i < len; i++)
    *((char*)(dst) + i) = *((char*)(src) + i);
  return (dst);
}

void *my_memset(void *b, int c, size_t len)
{
  size_t i;
  for (i = 0; i < len; i ++)
    *((char*)(b) + i) = c;
  return (b);
}

int my_strlen(char *str)
{
 int count;
   
 for (count = 0; *str; count++)
    str++;
  return (count);
}

int my_strcmp (const char *s1, const char *s2)
{
  while (*s1 == *s2++)
    if (*s1++ == 0)
      return (0);
  return (*s1 - *(s2 - 1));
}

int my_strncmp (const char *s1, const char *s2, size_t len)
{
  if (len == 0)
    return (0);
  do 
  {
    if (*s1 != *s2++)
      return (*s1 - *(s2 - 1));
    if (*s1++ == 0)
      break;
  } while (--len != 0);

  return (0);
}

int my_strcasecmp (const char *s1, const char *s2)
{
  const unsigned char *us1 = (const unsigned char *)s1;
  const unsigned char *us2 = (const unsigned char *)s2;

  while (my_tolower(*us1) == my_tolower(*us2++))
    if (*us1++ == '\0')
      return (0);

  return (my_tolower(*us1) - my_tolower(*--us2));
}

int my_strncasecmp (const char *s1, const char *s2, size_t n)
{
  const unsigned char *us1 = (const unsigned char *)s1;
  const unsigned char *us2 = (const unsigned char *)s2;

 if (n != 0) 
 {
   do 
   {
     if (my_tolower(*us1) != my_tolower(*us2++))
       return (my_tolower(*us1) - my_tolower(*--us2));
     if (*us1++ == '\0')
       break;
   } 
   while (--n != 0);
  }

  return (0);
}

char *my_strcpy(char *dst, const char *src)
{
  char *save = dst;
  for (; (*dst = *src); ++src, ++dst);
  return (save);
}

char *my_strcat(char *s, const char *append)
{
  char *save = s;
  for (; *s; ++s);
  while ((*s++ = *append++));
  return(save);
}

char *my_malloc_strcpy (char *str)
{
  char *buffer;
  size_t len;

  if (!str)
    return NULL;

  len = my_strlen (str);
  buffer = my_malloc (len + 1);
  my_memcpy (buffer, str, len + 1);

  return (buffer);
}

char *malloc_copie_cat (char *str1, char *str2)
{
  char *buf;
  buf = my_malloc ((my_strlen (str1) + my_strlen(str2) + 2) * sizeof (char));

  my_memset (buf, 0, my_strlen (str1) + my_strlen(str2) + 2);

  my_strcpy (buf, str1);
  if (str1[my_strlen (str1) - 1] != '/')
    buf[my_strlen (str1)] = '/';
  my_strcat (buf, str2);

  return (buf);
}

char *my_get_path_home (char *file)
{
  char *tmp, *tmp2;
  struct passwd *mypw;
  uid_t myid = getuid ();

  mypw = getpwuid (myid);

  if (!mypw)
    return (NULL);

  tmp = my_malloc_strcpy (mypw->pw_dir);
  tmp2 = my_malloc (sizeof (char) * (my_strlen (tmp) + my_strlen (file) + 2));
  sprintf (tmp2, "%s/%s", tmp, file);
  my_free (tmp);

  return (tmp2);
}

int my_isspace (char c)
{
  if (c == 011 || c == 040 || c == 012 || c == 014 || c == 015 || c == 13)
    return 1;
  else
    return 0;
}

char *strip_spaces (char *str)
{
  int i = 0;
  char *mot;

  if (str == NULL)
    return (NULL);

  if (*str == '\0')
    return (NULL);

  mot = my_malloc ((1 + my_strlen (str)) * sizeof (char));
  my_memset (mot, 0, my_strlen (str) + 1);

  while (*(str + i) == ' ') i ++;

  my_strcpy (mot, str + i);

  return (mot);
}

char *str_to_first_word (char *str)
{
  int i;
  char *mot, *mot2, *smot2;

  /* Well ... I'll strip ' ' ... */

  if ((mot2 = strip_spaces(str)) == NULL)
    return (NULL);

  smot2 = mot2;

  mot = my_malloc ((1 + my_strlen (mot2)) * sizeof (char));
  my_memset (mot, 0, my_strlen (mot2) + 1);

  for (i = 0 ; *mot2 != '\0' && !my_isspace(*mot2); i ++, mot2 ++)
    mot[i] = *mot2;

  my_free(smot2);

  if (my_strlen(mot) == 0) {
    my_free(mot);
    return (NULL);
  }

  return (mot);
}


/*
 * Transforme une chaine de caracteres en plusieurs dans un tableau.
 * Le separateur est donne en argument
 */
char **my_str_to_wordtab ( char * str , char sep )
{
  char **n;
  int mot = 0;
  int lettre = 0;
  int i = 0;

  n = my_malloc ((my_strlen(str)) * sizeof(char *));

  while (*(str + i))
  {
    if (*(str + i) == sep)
    {
      if (lettre)
      {
        n[mot][lettre] = 0;
        mot ++;
        lettre = 0;
      }
    }
    else
    {
      if (!lettre)
      {
        n[mot] = my_realloc(NULL, 0, 2);
        n[mot][0] = *(str + i);
        n[mot][1] = 0;
        lettre ++;
      }
      else
      {
        n[mot] = my_realloc(n[mot], lettre + 1, lettre + 2);
        n[mot][lettre] = *(str + i);
        n[mot][lettre + 1] = 0;
        lettre ++;
      }
    }

    i ++;
  }
  n[mot + 1] = NULL;

  return (n);
}

int my_tolower (int c)
{
  if (c >= 'A' && c <= 'Z')
    return (c - ('A' - 'a'));
  else
    return (c);
}

#ifdef NETWORK_DEBUG
int do_network_debug (char *fmt, ...)
{
  char buffer[1024];
  va_list truc;

  va_start(truc, fmt);

  vsprintf(buffer, fmt, truc);
  write (2, buffer, my_strlen (buffer));

  va_end (truc);

  return 0;
}
#endif

ssize_t my_read(int fd, void *buf, size_t count)
{
  ssize_t len;
  len = read (fd, buf, count);

  if (len == -1)
    printf_cl("read: %s\n", strerror(errno));

#ifdef NETWORK_DEBUG
  if (len > 0)
    fprintf(stderr, "[<=] (%s)\n", (char*)buf);
#endif

  return (len);
}

ssize_t my_write(int fd, const void *buf, size_t count)
{
  ssize_t len;

#ifdef NETWORK_DEBUG
  fprintf(stderr, "[=>] (%s)\n", (char*)buf);
#endif

  len = write (fd, buf, count);
  if (len == -1)
    printf_cl("write: %s\n", strerror(errno));

  return (len);
}


