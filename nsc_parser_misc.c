/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <time.h>

#include "config.h"

#include "my_functions.h"
#include "nsc_functions.h"
#include "nsc_code.h"
#include "nsc_user.h"
#include "nsc_functions.h"
#include "nsc_parsers.h"
#include "nsc_conf.h"
#include "nsc_commands.h"
#include "nsc_contact.h"


int parse_user_cmd (char *buffer)
{
  int id, weak, trusted, ret, readchar;
  char user[33], his_host[17], type_host[256], location[256], groupe[33];
  char cmd[256];
  char *last, *tmpstr, *tmpbuf, *t2;

  /* for status */
  char status[33];
  char ctimebuffer[27];
  int ts;

  struct ignore_list_t *st_ign;
  
  /*
   * user_cmd 0:mail:9/9:_deamon:: | new_mail -f beauma_p@epita.fr %28bisous%29
   */

  ret = sscanf (buffer,"user_cmd %i:mail:%i/%i:%32[^:]:: | new_mail -f %255[^ ] %255[^ ] %n",
          &id, &weak, &trusted, user, location, cmd, &readchar);

  if (ret == 6)
  {
    tmpbuf = ns_decode (cmd);
    printf_cl ("mail from %s %s\n", location, tmpbuf);
    my_free(tmpbuf);
    return (0);
  }

  /*
   * user_cmd 105:user:3/1:sausse_c@10.42.110.37:NetBsd_wse:sm-p10_r4p7:ept1 | msg bisou
   */

  ret = sscanf (buffer,"user_cmd %i:user:%i/%i:%32[^@]@%16[^:]:%255[^:]:%255[^:]:%32[^ ] | %255[^ ] %n",
          &id, &weak, &trusted, user, his_host, type_host, 
          location, groupe, cmd, &readchar);

  if (ret == 9)
  {
    if (!my_strncasecmp (cmd, "msg", my_strlen ("msg")))
    {
      if ((st_ign = is_ignore(user)) != NULL) {
        /* envoit d'un message */
        if (st_ign->message != NULL) {
          tmpbuf = my_malloc (my_strlen(user) + my_strlen(st_ign->message)+2);
          sprintf(tmpbuf, " %s %s", user, st_ign->message);
          cmd_user_message(tmpbuf);
          my_free(tmpbuf);
        }
        return(0);
      }
      tmpbuf = ns_decode (buffer + readchar);

      tmpstr = tmpbuf;
      last = NULL;
      while ((tmpstr = strchr(tmpstr + 1, 'd')))
      {
	if (strncmp(tmpstr, "dst=", 4) == 0)
	{
	  last = tmpstr - 1;
	}
      }
      *last = '\0';

      printf_cl ("msg > %s > %s\n", user, tmpbuf);
      my_free (tmpbuf);

      /* si status == away ou status == lock, on envoit un auto responder */
       if((nsc_int_var(GET_CONF, CNX_STATUS, 0) == STATUS_LOCK)
       && (t2=nsc_string_var(GET_CONF, CONF_AUTO_MESSAGE_LOCK,NULL)) != NULL){
        tmpbuf = my_malloc (my_strlen(user) + my_strlen(t2) + 3);
        sprintf(tmpbuf, " %s %s", user, t2);
        cmd_user_message(tmpbuf);
        my_free(tmpbuf);
      }

       if((nsc_int_var(GET_CONF, CNX_STATUS, 0) == STATUS_AWAY)
       &&(t2 = nsc_string_var(GET_CONF, CONF_AUTO_MESSAGE_AWAY,NULL)) != NULL){
        tmpbuf = my_malloc (my_strlen(user) + my_strlen(t2) + 3);
        sprintf(tmpbuf, " %s %s", user, t2);
        cmd_user_message(tmpbuf);
        my_free(tmpbuf);
      }

      return (0);
    }

    if (!my_strncasecmp (cmd, "login", my_strlen ("login")))
    {
      t2 = ns_decode (location);
      printf_cl ("join > %s (%s:%s)\n", user, his_host, t2);
      my_free (t2);
      return (0);
    }

    else if (!my_strncasecmp (cmd, "logout", my_strlen ("logout")))
    {
      t2 = ns_decode (location);
      printf_cl ("quit > %s (%s:%s)\n", user, his_host, t2);
      my_free (t2);
      return (0);
    }

    else if (!my_strncasecmp (cmd, "state", my_strlen ("state")))
    {
      if(nsc_int_var(GET_CONF, CONF_SHOWSTATUSCHANGES, 0) == 0)
        return 0;

      ret = sscanf (buffer + readchar, "%32[^:]:%i", status, &ts);

      if (ret == 2)
      {
        ctime_r ((const time_t*)&ts, ctimebuffer);
        ctimebuffer[24] = 0;
        printf_cl ("status > %8s > %7s (%s)\n", user, status, ctimebuffer);
      }
      else
        printf_cl ("status > %8s > %s\n", user, buffer + readchar);

      return (0);
    }

    else if (!my_strncasecmp (cmd, "typing", my_strlen ("typing")) || \
	     !my_strncasecmp (cmd, "dotnetSoul_", my_strlen ("dotnetSoul_")))
    {
      return (0);
    }
  }

  return (-1);
}

int parse_user_list (char *buffer)
{
  char user[33], his_ip[17], type_host[256], location[256], groupe[33];
  char  user_data[256],  status[256], status_buf[256];
  int id, weak, trust, lu, ret;
  int tscnx;
  int tsstate;
  int ts;
  char *t1, *t2;

 /*
  * 873 quelen_b 213.36.76.161 1044207167 1044212559 3 1 ~ midlab ept3 actif:1044212559 geronimo-0.7.5%20%5Btoto42%5D
  */

  ret = sscanf (buffer, "%i %32[^ ] %16[^ ] %i %i %i %i %255[^ ] %255[^ ] "
                     "%32[^ ] %255[^ ] %255[^\n]%n",
          &id, user, his_ip, &tscnx, &tsstate, &weak, &trust, type_host,
          location, groupe, status, user_data, &lu);

  if (ret == 12)
  {
    t1 = ns_decode (user_data);
    t2 = ns_decode (location);
 
    ret = sscanf (status, "%32[^:]:%i", status_buf, &ts);

    if (ret == 2)
      printf_cl ("%s at %s (%s) is using %s\n", user, t2, status_buf, t1);
    else
      printf_cl ("%s at %s (%s) is using %s\n", user, t2, status, t1);

    if (count_list == 0) {
      printf_cl ("Connected at: %s", ctime((const time_t*)&tscnx));
      printf_cl ("-- Status at: %s", ctime((const time_t*)&tsstate));
    } else {
      if (nsc_int_var(GET_CONF, CONF_SHOWSTATUSLIST, 0) == 1) {
        printf_cl ("-- Status at: %s", ctime((const time_t*)&tsstate));
      }
    }

    my_free (t1);
    my_free (t2);
    return (0);
  }

  return (-1);
}

int parse_cmd_end (char *buffer)
{
  if (my_strncmp(buffer, "rep 00", 6) != 0)
    return (-1);

  /* Right, we got end of command. So we print end of what. */
  if (last_cmd == CMD_WHOIS) {
    printf_cl ("End of whois command.\n");
    count_whois --;
    if (count_whois == 0)
      last_cmd = 0;
  } else if (last_cmd == CMD_LIST) {
    printf_cl ("End of list command.\n");
    count_list --;
    if (count_list == 0)
      last_cmd = 0;
  } else {
    printf_cl ("End of command.\n");
  }

  return (0);
}
