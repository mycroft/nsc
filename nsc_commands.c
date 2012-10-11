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
#include <time.h>

#include "my_functions.h"
#include "nsc_main.h"
#include "nsc_functions.h"
#include "nsc_commands.h"
#include "nsc_conf.h"
#include "nsc_code.h"
#include "nsc_contact.h"
#include "nsc_user.h"

#include "config.h"

command commands_list[] =
{
  { "help" ,     cmd_help,         "help", COM_NOCOMP },
  { "quit",      cmd_exit,         "exit nsc", COM_NOCOMP },
  { "exit" ,     cmd_exit,         "exit nsc", COM_NOCOMP },
  { "reload_conf", cmd_reload_config, "reload configuration", COM_NOCOMP },
  { "save_conf", cmd_save_config,  "save configuration", COM_NOCOMP },
  { "reco" ,     cmd_reco,         "reconnect", COM_NOCOMP },
  { "deco" ,     cmd_deco,         "disconnect", COM_NOCOMP },
  { "connect" ,  cmd_connect,      "connect", COM_NOCOMP },
  { "raw",       cmd_raw,          "send a raw message to server", COM_NOCOMP },
  { "msg",       cmd_user_message, "send a message to someone", COM_NICK },
  { "query",     cmd_query,        "start/end a query with someone", COM_NOCOMP },
  { "state",     cmd_user_state,   "change status", COM_STATE },
  { "whois",     cmd_user_whois,   "whois someone", COM_NICK },
  { "list",      cmd_user_list,    "list your friends", COM_NOCOMP },
  { "add_contact", cmd_add_contact,"add a friend", COM_NOCOMP },
  { "rem_contact", cmd_rem_contact,"remove a friend", COM_NICK },
  { "list_contact", cmd_list_contact,"list friends", COM_NOCOMP },
  { "add_ignore", cmd_add_ignore,  "add a foe", COM_NOCOMP },
  { "rem_ignore", cmd_rem_ignore,  "remove a foe", COM_NICK },
  { "list_ignore", cmd_list_ignore,"list foes", COM_NOCOMP },
  { "away",      cmd_away,         "become away", COM_NOCOMP },
  { "back",      cmd_actif,        "become actif", COM_NOCOMP },
  { "actif",     cmd_actif,        "become actif", COM_NOCOMP },
  { "version",   cmd_version,      "show version", COM_NOCOMP },
  { "userdata",  cmd_userdata,     "change user data", COM_NOCOMP },
  { "location",  cmd_location,     "change user location", COM_NOCOMP },
  { "", NULL, "", COM_NOCOMP }
};

int count_whois = 0;
int count_list = 0;
int last_cmd = 0;

pcommand get_command_struct (char *text)
{
  command *tmp;

  for (tmp = commands_list; tmp->cmd_handler; tmp ++) {
    if (my_strncmp(text, tmp->name, my_strlen (tmp->name)) == 0) {
      return (tmp);
	}
  }

  return (NULL);
}

int cmd_help (char *str)
{
  command *tmp;

  for (tmp = commands_list; tmp->cmd_handler; tmp ++)
  {
    if (my_strlen(tmp->name) < 6)
      printf ("> %s\t\t-\t%s\n", tmp->name, tmp->description);
    else
      printf ("> %s\t-\t%s\n", tmp->name, tmp->description);
  }

  return 0;
}

int cmd_exit (char *str)
{
  main_prepare_exit (); 

  return 0;
}

int cmd_reco (char *str)
{
  main_cc ();
  main_lc ();

  return 0;
}

int cmd_deco (char *str)
{
  main_cc ();

  return 0;
}

int cmd_connect (char *str)
{
  main_lc ();

  return 0;
}


int cmd_raw (char *str)
{
  int l;

  if (*str == '\0')
    return (-1);

  if ( *(str + 1) == '\0')
    return (-1);

  printf ("Raw sending: [%s]\n", str + 1);
  l = nsc_int_var (GET_CONF, CNX_CLIENT_FD, 0);
  writebuffer (l, "%s\n", str + 1);

  return 0;
}

int cmd_user_message (char *str)
{
  int l;
  char *user;
  char *message;
  char *tmpbuf;

  /* That code is ugly as hell */

  if (*str == '\0')
    return (-1);
  
  if ( *(str + 1) == '\0')
    return (-1);

  user = str_to_first_word(str + 1);
  if (!user)
    return (0);

  message = str + 1 + my_strlen(user);

  if (*message == '\0') 
  {
    my_free (user);
    return (-1);
  }	

  if (*(message + 1) == '\0')
  {
    my_free (user);
    return (-1);
  }

  l = nsc_int_var (GET_CONF, CNX_CLIENT_FD, 0);

  tmpbuf = ns_encode (message + 1);
  if (nsc_int_var(GET_CONF, CNX_TYPE, 0) == TYPE_EXTERNE)
    writebuffer (l, "user_cmd msg_user {%s} msg %s\n", user, tmpbuf);
  else
    writebuffer (l, "cmd msg_user {%s} msg %s\n", user, tmpbuf);

  my_free (tmpbuf);
  my_free (user);

  return 0;
}

int cmd_user_state (char *str)
{
  int l;
  char *tmpstr;
  time_t timestamp = time (NULL);

  tmpstr = strip_spaces (str);
  if (tmpstr == NULL || my_strlen (tmpstr) == 0) {
    if (tmpstr != NULL) 
      my_free(tmpstr);
    tmpstr = NULL;
  }

  if (tmpstr == NULL) {
    switch(nsc_int_var(GET_CONF, CNX_STATUS, 0)) {
    case STATUS_AWAY: tmpstr = "away"; break; 
    case STATUS_LOCK: tmpstr = "lock"; break; 
    case STATUS_IDLE: tmpstr = "idle"; break; 
    case STATUS_ACTIF: tmpstr = "actif"; break; 
    default: tmpstr = "unknown"; break;
    }
    printf_cl("Current status: %s\n", tmpstr);
    return (0);
  } else {
    my_free(tmpstr);
    tmpstr = NULL;
  }


  tmpstr = str_to_first_word (str);

  l = nsc_int_var (GET_CONF, CNX_CLIENT_FD, 0);
  writebuffer (l, "state %s:%i\n", tmpstr, timestamp);

  /* On change la config */
  if (!my_strcmp(tmpstr, "away")) {
    nsc_int_var(SET_CONF, CNX_STATUS, STATUS_AWAY);
  } else if (!my_strcmp(tmpstr, "lock")) {
    nsc_int_var(SET_CONF, CNX_STATUS, STATUS_LOCK);
  } else if (!my_strcmp(tmpstr, "idle")) {
    nsc_int_var(SET_CONF, CNX_STATUS, STATUS_IDLE);
  } else if (!my_strcmp(tmpstr, "actif")) {
    nsc_int_var(SET_CONF, CNX_STATUS, STATUS_ACTIF);
  }

  my_free (tmpstr);

  return (0);
}

int cmd_user_whois (char *str)
{
  int l;
  char *tmpstr;
  if (!str)
    return (-1);

  tmpstr = strip_spaces (str);

  last_cmd = CMD_WHOIS;
  count_whois ++;
  l = nsc_int_var (GET_CONF, CNX_CLIENT_FD, 0);
  if (nsc_int_var(GET_CONF, CNX_TYPE, 0) == TYPE_EXTERNE)
    writebuffer (l, "list_users %s\n", tmpstr);
  else
    writebuffer (l, "cmd who  %s\n", tmpstr);

  my_free (tmpstr);
  return (0);
}

int cmd_user_list (char *str)
{
  int l;
  char *t;

  t = build_accolades_contact_list();

  count_list ++;
  last_cmd = CMD_LIST;

  l = nsc_int_var (GET_CONF, CNX_CLIENT_FD, 0);
  if (nsc_int_var(GET_CONF, CNX_TYPE, 0) == TYPE_EXTERNE)
    writebuffer (l, "list_users %s\n", t);
  else
    writebuffer (l, "cmd who  %s\n", t);

  my_free (t);
  return (0);
}

int cmd_reload_config(char *str)
{
  nsc_free_configuration();
  nsc_load_configuration();
  return 0;
}

int cmd_save_config(char *str)
{
  nsc_save_configuration();
  return 0;
}

int cmd_add_contact(char *str)
{
  char *tmpstr;

  tmpstr = strip_spaces (str);

  add_contact(tmpstr);

  my_free (tmpstr);
  return 0;
}

int cmd_rem_contact(char *str)
{
  char *tmpstr;

  tmpstr = strip_spaces (str);
 
  rem_contact(tmpstr);

  my_free (tmpstr);
  return 0;
}

int cmd_list_contact(char *str)
{
  int i = 0;
  char *t;

  printf_cl("Contact list:\n");

  while ((t = get_next_contact_list_name()) != NULL) {
    i ++;
    printf_cl("%.2d/ %s\n", i, t);
  }
  printf_cl("%d entries in contact list.\n", i);

  return 0;
}

int cmd_add_ignore(char *str)
{
  char *tmpstr;

  tmpstr = strip_spaces (str);

  add_ignore(tmpstr);

  my_free (tmpstr);
  return 0;
}

int cmd_rem_ignore(char *str)
{
  char *tmpstr;

  tmpstr = strip_spaces (str);

  rem_ignore(tmpstr);

  my_free (tmpstr);
  return 0;
}

int cmd_list_ignore(char *str)
{
  int i = 0;
  struct ignore_list_t *t;

  printf_cl("Ignore list:\n");

  while ((t = get_next_ignore_list_name()) != NULL) {
    i ++;
    if (t->message != NULL)
      printf_cl("%.2d/ %s (%s)\n", i, t->name, t->message);
    else
      printf_cl("%.2d/ %s\n", i, t->name);
  }
  printf_cl("%d entries in ignore list.\n", i);

  return 0;
}

int cmd_away(char *str)
{
  cmd_user_state ("away");

  return 0;
}

int cmd_actif(char *str)
{
  cmd_user_state ("actif");

  return 0;
}

int cmd_version(char *str)
{
  printf_cl("You re using nsc v. %s\n", VERSION);

  return 0;
}

int cmd_query(char *str)
{
  char *tmpstr;
  char *q;

  tmpstr = str_to_first_word(str);

  if (tmpstr != NULL && my_strlen(tmpstr) == 0) {
    my_free (tmpstr);
    tmpstr = NULL;
  }
//  tmpstr = strip_spaces (str);

  if (tmpstr) {
    q = nsc_string_var (GET_CONF, CNX_QUERY, NULL);
    if (q) {
      printf_cl("End of query with [%s]\n", q);
    }
    nsc_string_var (SET_CONF, CNX_QUERY, tmpstr);
    printf_cl("Start a query with [%s]\n", tmpstr);

    my_free (tmpstr);
  } else {
    /* End of query */
    q = nsc_string_var (GET_CONF, CNX_QUERY, NULL);
    if (q) {
      printf_cl("End of query with [%s]\n", q);
      nsc_string_var (SET_CONF, CNX_QUERY, NULL);
    } else {
      printf_cl("No query.\n");
    }
  }

  return 0;
}

int cmd_userdata(char *str)
{
  int l;
  char *tmpstr, *ctmpstr;

  tmpstr = strip_spaces (str); 

  if (tmpstr != NULL && my_strlen(tmpstr) == 0) {
    my_free(tmpstr);
    tmpstr = NULL;
  }

  if (tmpstr == NULL) {
    tmpstr = nsc_string_var(GET_CONF, CONF_USERDATA, NULL);
    printf_cl("Current data: %s\n", tmpstr);
    return 0;
  }

  nsc_string_var(SET_CONF, CONF_USERDATA, tmpstr);
  printf_cl("New data: %s\n", tmpstr);

  l = nsc_int_var (GET_CONF, CNX_CLIENT_FD, 0);

  ctmpstr = ns_encode (tmpstr);
  if (nsc_int_var(GET_CONF, CNX_TYPE, 0) == TYPE_EXTERNE)
    writebuffer (l, "user_cmd user_data %s\n", ctmpstr);
  else
    writebuffer (l, "cmd user_data %s\n", ctmpstr);
  my_free (tmpstr);
  my_free (ctmpstr);

  return 0;
}

int cmd_location(char *str)
{
  int l;
  char *tmpstr;

  tmpstr = strip_spaces (str);

  if (tmpstr != NULL && my_strlen(tmpstr) == 0) {
    my_free(tmpstr);
    tmpstr = NULL;
  }

  if (tmpstr == NULL) {
    tmpstr = nsc_string_var(GET_CONF, CONF_LOCATION, NULL);
    printf_cl("Current location: %s\n", tmpstr);
    return 0;
  }

  nsc_string_var(SET_CONF, CONF_LOCATION, tmpstr);
  printf_cl("New location: %s\n", tmpstr);
  printf_cl("Please reco to use that location.\n");

  my_free (tmpstr);

  return 0;
}
