/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdarg.h>
#include "my_functions.h"
#include "nsc_main.h"

ssize_t writebuffer (int sock, char *fmt, ...)
{
  char buf[2048];
  va_list ap;
  ssize_t ret;

  va_start (ap, fmt);

  my_memset (buf, 0, sizeof(buf));
  vsnprintf (buf, sizeof (buf) - 1, fmt, ap); 

  ret = my_write (sock, buf, my_strlen (buf)); 

  va_end (ap);

  return (ret);
}

ssize_t writebuffer_no_fmt (int sock, char *str)
{
  int ret;
  ret = my_write (sock, str, my_strlen (str));
  return (ret);
}

