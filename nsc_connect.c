/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>

#include "nsc_main.h"
#include "my_functions.h"
#include "nsc_functions.h"
#include "get_next_line.h"
#include "nsc_conf.h"
#include "nsc_commands.h"
#include "nsc_parsers.h"
#include "nsc_auth.h"
#include "nsc_user.h"
#include "nsc_contact.h"

/* This function ll handle recieved data by status */

int nsc_recv_data (void)
{
  int tmp_fd;
  char *buffer;
  char *tmpstr;
  char *bigbuf;
  char *last;
  int ret = 0;

  tmp_fd = nsc_int_var (GET_CONF, CNX_CLIENT_FD, 0);

debut:
  buffer = get_next_line (tmp_fd);

  if (buffer == NULL)
  { /* Humm */
    return (-1);
  }

  switch (nsc_int_var (GET_CONF, CNX_STATUS, 0))
  {
    case STATUS_NOT_CONNECTED:
      if (parse_hello (buffer) == -1)
        ret = -1;
      else
      {
        nsc_int_var (SET_CONF, CNX_STATUS, STATUS_NOT_AUTH);
        printf_cl ("asking for auth.\n");
        ret = writebuffer (tmp_fd, NSC_ASK_AUTH);
      }
    break;

    case STATUS_NOT_AUTH:
      if (parse_validity (buffer) == -1)
        ret = -1;
      else
      {
	nsc_int_var (SET_CONF, CNX_STATUS, STATUS_ATTACH_SENT);
        printf_cl ("sending auth. message\n");
        /* build auth message and send it */
        tmpstr = build_auth_message ();
        ret = writebuffer_no_fmt (tmp_fd, tmpstr);
        my_free (tmpstr);
      }
    break;

    case STATUS_AUTH_SENT:
      if (parse_validity (buffer) == -1)
        ret = -1;
      else
      {
        nsc_int_var (SET_CONF, CNX_STATUS, STATUS_ATTACH_SENT);
        if (nsc_int_var(GET_CONF, CNX_TYPE, 0) == TYPE_EXTERNE) {
          /* sending 'attach' command */
          printf_cl ("sending 'attach' command\n");
          ret = writebuffer_no_fmt (tmp_fd, "attach\n");
        }
      }
    break;

    case STATUS_ATTACH_SENT:
      if (parse_validity (buffer) == -1)
        ret = -1;
      else
      {
        printf_cl ("auth. is ok\n");
        printf_cl ("sending contact list watchlog\n");
        tmpstr = build_accolades_contact_list ();
        bigbuf = my_malloc (sizeof(char) * (my_strlen (tmpstr) + 128));

        if (nsc_int_var(GET_CONF, CNX_TYPE, 0) == TYPE_EXTERNE)
          sprintf (bigbuf, "user_cmd watch_log_user %s\n", tmpstr); 
        else
          sprintf (bigbuf, "cmd watch_log_user %s\n", tmpstr);

        ret = writebuffer_no_fmt (tmp_fd, bigbuf);
        my_free (tmpstr);
        my_free (bigbuf);

        printf_cl ("setting state at actif\n");
        cmd_user_state ("actif");
        ret = 0;
      }
    break;

    case STATUS_CONNECTION:
    case STATUS_ACTIF:
    case STATUS_AWAY:
    case STATUS_IDLE:
    case STATUS_LOCK:
      if ((ret = parse_ping (buffer)) != -1)
      {
        if (nsc_int_var(GET_CONF, CNX_TYPE, 0) == TYPE_EXTERNE)
          ret = writebuffer (tmp_fd, "ping %i\n", ret);
        else
          ret = writebuffer (tmp_fd, "cmd ping %i\n", ret);
      } 
      else if ((ret = parse_user_cmd (buffer)) != -1)
      {
      }
      else if ((ret = parse_user_list (buffer)) != -1)
      { 
      }
      else if ((ret = parse_cmd_end (buffer)) != -1)
      {
      }
      else
        printf_cl ("!!! Not yet handled: [%s]\n", buffer);

      ret = 0;
    break;
  }

  my_free (buffer);

  if (is_there_get_next_data())
    goto debut;

  return (ret);
}
