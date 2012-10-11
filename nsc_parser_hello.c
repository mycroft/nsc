/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include "my_functions.h"
#include "nsc_parsers.h"
#include "nsc_conf.h"

int parse_hello (char *buffer)
{
  int ret;
  int tmp_id, tmp_port, tmp_ts;
  char tmp_chal[33];
  char tmp_ip[16];

/* Jai du recevoir ca:
 * salut 167 52ccd4a122d3b25850111937b281d249 212.83.191.227 32849 1037344836 
 */

  ret = sscanf (buffer, "salut %d %32[^ ] %16[^ ] %d %d\n",
                &tmp_id,
                tmp_chal,
                tmp_ip,
                &tmp_port,
                &tmp_ts);

  if (ret == 5)
  {
    nsc_int_var (SET_CONF, CNX_CLIENT_PORT, tmp_port);
    nsc_int_var (SET_CONF, CNX_SESSION_ID,  tmp_id);
    nsc_int_var (SET_CONF, CNX_SERVER_TS,   tmp_ts);
    nsc_string_var (SET_CONF, CNX_CLIENT_HOST, tmp_ip);
    nsc_string_var (SET_CONF, CNX_CHAL_STR, tmp_chal);
  }
  else
    return (-1);

  return (0);
}

int parse_validity (char *buf)
{
  if (!buf)
    return (-1);

  if (!my_strncmp (buf, "rep 002 -- cmd end", my_strlen (buf) - 1))
    return (0);

  return (-1);
}

