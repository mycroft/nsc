/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "my_functions.h"
#include "nsc_conf.h"
#include "nsc_user.h"
#include "nsc_commands.h"

struct in_addr resolv(char *hostname)
{
  struct in_addr in;
  struct hostent *hp;

  if (!inet_aton(hostname, &in))
  {
    if ((hp = gethostbyname(hostname)))
      my_memcpy(&in.s_addr, hp->h_addr, hp->h_length);
    else
    {
      printf_cl("Can't resolv hostname ?!\n");
      exit(-1);
    }
  }
  return in;
}

int nsc_connect_socket (void)
{
  int sock;
  int ret;
  char *tmp_serv;
  unsigned short tmp_port;
  struct sockaddr_in sai;
  struct sockaddr_un sua; 
  char path[PATH_MAX];

  if ((tmp_serv = getenv("NS_USER_LINK")) != NULL) {
    sprintf(path, "%s/.ns/%s", getenv("HOME"), tmp_serv);
    sock = socket (PF_LOCAL, SOCK_STREAM, 0);
    my_strcpy(sua.sun_path, path);

    ret = connect (sock, (struct sockaddr *)&sua, sizeof(sua));

    if (ret != -1) {
      cmd_user_state(" actif");
      nsc_int_var(SET_CONF, CNX_TYPE, TYPE_INTERNE);
    }

  } else {

    if (!(tmp_serv = nsc_string_var (GET_CONF, CONF_SERVER_HOST, NULL))
       || !(tmp_port = nsc_int_var (GET_CONF, CONF_SERVER_PORT, 0)))
    {
      printf_cl ("Lack of configuration.\n");
      return (0);
    }

    sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
    {
      printf_cl ("Socket error (socket(2))\n");
      return (-1);
    }

    sai.sin_family = AF_INET;
    sai.sin_addr = resolv (tmp_serv);
    sai.sin_port = htons (tmp_port);

    ret = connect (sock, (struct sockaddr*)&sai, sizeof (struct sockaddr));
    if (ret != -1) {
      nsc_int_var(SET_CONF, CNX_TYPE, TYPE_EXTERNE);
    }
  }

  if (ret == -1)
  {
    printf_error ("Connect error (connect(2))\n");
    close (sock);
    return (-1);
  }

  nsc_int_var (SET_CONF, CNX_CLIENT_FD, sock);

  return (sock);
}

int nsc_close_socket (void)
{
  int sock;

  sock = nsc_int_var (GET_CONF, CNX_CLIENT_FD, 0);
  if (sock)
  {
    nsc_int_var (SET_CONF, CNX_CLIENT_FD, 0);
    close (sock);
  }
  else
    return (-1);

  return (sock);
}
