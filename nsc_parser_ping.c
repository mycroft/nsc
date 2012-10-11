/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>

#include "config.h"

#include "my_functions.h"
#include "nsc_functions.h"
#include "nsc_parsers.h"
#include "nsc_conf.h"

int parse_ping (char *buffer)
{
  int time;
  int ret;

  /* J ai du recevoir ca:
   * "ping 600 "
   */
  ret = sscanf (buffer, "ping %i", &time);

  if (ret == 1)
    return (time);
  else
    return (-1);

  /* Can't appen */
  return (0);
}

int send_ping (int sec)
{
  int ret = 0;
  static int secondes = 0;
  int tmp_fd;

  secondes += sec;

#ifdef SENDPING
  if (secondes >= SENDPING)
#else
  if (secondes >= 300)
#endif
  {
    tmp_fd = nsc_int_var (GET_CONF, CNX_CLIENT_FD, 0);
    if (tmp_fd)
    {
      if (nsc_int_var(GET_CONF, CNX_TYPE, 0) == TYPE_EXTERNE)
        ret = writebuffer (tmp_fd, "ping %i \n", sec);
      else
        ret = writebuffer (tmp_fd, "cmd ping %i \n", sec);
    }
  }

  return (ret);
}
