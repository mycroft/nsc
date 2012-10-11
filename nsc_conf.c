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
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "nsc_conf.h"
#include "my_functions.h"
#include "nsc_user.h"
#include "get_next_line.h"
#include "nsc_contact.h"

static char * nsc_server_host = NULL;
static unsigned short nsc_server_port = 0;
static char * nsc_client_host = NULL;
static unsigned short nsc_client_port = 0;

static char * nsc_login = NULL;
static char * nsc_pass = NULL;

static char * nsc_location = NULL;
static char * nsc_userdata = NULL;

static char * nsc_chalenge = NULL;
static char * nsc_query = NULL;

static int nsc_status = STATUS_NOT_CONNECTED;
static int client_fd = 0;
static int session_id = 0;
static int timestamp = 0;

static char * auto_message_lock = NULL;
static char * auto_message_away = NULL;

static unsigned char nsc_type_cnx = 0;

static unsigned char nsc_show_status_on_list = 0;
static unsigned char nsc_show_status_changes = 0;

int nsc_int_var ( char action , char nom , int value )
{
  int ret_value = -1;
#ifdef DEBUG
  printf_cl ("nsc_int_var: action: %i / nom: %i / value: %i\n", 
             action, nom, value);
#endif

  switch (nom)
  {
    case CONF_SERVER_PORT:
      if (action == SET_CONF)
        nsc_server_port = value;
      else
        ret_value = nsc_server_port;
    break;

    case CNX_CLIENT_FD:
      if (action == SET_CONF)
        client_fd = value;
      else
        ret_value = client_fd;
    break;

    case CNX_CLIENT_PORT:
      if (action == SET_CONF)
        nsc_client_port = value;
      else
        ret_value = nsc_client_port;
    break;

    case CNX_SESSION_ID:
      if (action == SET_CONF)
        session_id = value;
      else
        ret_value = session_id; 
    break;

    case CNX_SERVER_TS:
      if (action == SET_CONF)
        timestamp = value;
      else
        ret_value = timestamp; 
    break;

    case CNX_STATUS:
      if (action == SET_CONF)
        nsc_status = value;
      else
        ret_value = nsc_status;
    break;

    case CNX_TYPE:
      if (action == SET_CONF)
        nsc_type_cnx = value;
      else
        ret_value = nsc_type_cnx;
     break;

     case CONF_SHOWSTATUSLIST:
       if (action == SET_CONF)
         nsc_show_status_on_list = value;
       else
         ret_value = nsc_show_status_on_list; 
     break;

     case CONF_SHOWSTATUSCHANGES:
       if (action == SET_CONF)
         nsc_show_status_changes = value;
       else 
         ret_value = nsc_show_status_changes;
     break;
  }

  return (ret_value);
}

char *nsc_string_var ( char action , char nom , char *value )
{
  char *ret_value = NULL;
  char *new_value = NULL;

#ifdef DEBUG
  printf_cl ("nsc_stringvar: action: %i / nom: %i / value: %s\n",
              action, nom, value);
#endif

  if ( action == SET_CONF )
  {
    if (value)
      new_value = my_malloc_strcpy (value);
    else
      new_value = NULL;
  }

  switch (nom)
  {
    case CONF_SERVER_HOST:
      if (action == SET_CONF)
      {
        if (nsc_server_host)
          my_free (nsc_server_host);
        nsc_server_host = new_value;
      }
      else
        ret_value = nsc_server_host;
    break;
    case CONF_LOGIN:
      if (action == SET_CONF)
      {
        if (nsc_login)
          my_free (nsc_login);
        nsc_login = new_value;
      }
      else
        ret_value = nsc_login;
    break;
    case CONF_PASS:
      if (action == SET_CONF)
      {
        if (nsc_pass)
          my_free (nsc_pass);
        nsc_pass = new_value;
      }
      else
        ret_value = nsc_pass;
    break;
    case CONF_LOCATION:
      if (action == SET_CONF)
      {
        if (nsc_location)
          my_free (nsc_location);
        nsc_location = new_value;
      }
      else
        ret_value = nsc_location;
    break;
    case CONF_USERDATA:
      if (action == SET_CONF)
      {
        if (nsc_userdata)
          my_free (nsc_userdata);
        nsc_userdata = new_value;
      }
      else
        ret_value = nsc_userdata;
    break;
    case CNX_CLIENT_HOST:
      if (action == SET_CONF)
      {
        if (nsc_client_host)
          my_free (nsc_client_host);
        nsc_client_host = new_value;
      }
      else
        ret_value = nsc_client_host;
    break;
    case CNX_CHAL_STR:
      if (action == SET_CONF)
      {
        if (nsc_chalenge)
          my_free (nsc_chalenge);
        nsc_chalenge = new_value;
      } 
      else
        ret_value = nsc_chalenge;
    break;
    case CNX_QUERY:
      if (action == SET_CONF)
      {
        if (nsc_query)
          my_free (nsc_query);
        nsc_query = new_value;
      }
      else
        ret_value = nsc_query;
    break;

    case CONF_AUTO_MESSAGE_AWAY:
      if (action == SET_CONF)
      {
        if(auto_message_away != NULL)
          my_free(auto_message_away);
        auto_message_away = new_value;
      }
      else
        ret_value = auto_message_away;
    break;
    case CONF_AUTO_MESSAGE_LOCK:
      if (action == SET_CONF) 
      {
        if(auto_message_lock != NULL)
          my_free(auto_message_lock);
        auto_message_lock = new_value;
      }
      else
        ret_value = auto_message_lock;
    break;
  }

  return (ret_value);
}

int nsc_free_configuration ( void )
{
  nsc_string_var (SET_CONF, CONF_SERVER_HOST, NULL);
  nsc_string_var (SET_CONF, CONF_LOGIN, NULL);
  nsc_string_var (SET_CONF, CONF_PASS, NULL);
  nsc_string_var (SET_CONF, CONF_LOCATION, NULL);
  nsc_string_var (SET_CONF, CONF_USERDATA, NULL);
  nsc_string_var (SET_CONF, CNX_CLIENT_HOST, NULL);
  nsc_string_var (SET_CONF, CNX_CHAL_STR, NULL);
  nsc_string_var (SET_CONF, CNX_QUERY, NULL);
  nsc_string_var (SET_CONF, CONF_AUTO_MESSAGE_AWAY, NULL);
  nsc_string_var (SET_CONF, CONF_AUTO_MESSAGE_LOCK, NULL);
  nsc_int_var (SET_CONF, CONF_SERVER_PORT, 0);
  nsc_int_var (SET_CONF, CONF_SHOWSTATUSLIST, 0);
  nsc_int_var (SET_CONF, CONF_SHOWSTATUSCHANGES, 0);

  flush_contact_list();
  flush_ignore_list();

  return (0);
}

int nsc_load_configuration (void)
{
  char *file = my_get_path_home (CONFIGURATION_FILE);
  char *r;
  int hand;
  int cont;

  hand = open (file, O_RDONLY);

  if (hand == -1)
  {
    printf_cl("open: %s\n", strerror(errno));
    my_free (file);
    return (-1);
  }

  do
  {
    r = get_next_line (hand);
    cont = 0;
    if (r)
    {
#define CONF_LINE_SERVER "server "
#define CONF_LINE_PORT   "port "
#define CONF_LINE_LOGIN  "login "
#define CONF_LINE_PASSWD "pass "
#define CONF_LINE_LOCATION "location "
#define CONF_LINE_USERDATA "userdata "
#define CONF_LINE_CONTACT "contact "
#define CONF_LINE_IGNORE "ignore "
#define CONF_LINE_AWAY_MSG "away_message "
#define CONF_LINE_LOCK_MSG "lock_message "
#define CONF_LINE_SHOWSTATUSLIST "set showstatusonlist"
#define CONF_LINE_SHOWSTATUSCHANGES "set showstatuschanges"

      if (!my_strncasecmp (r, CONF_LINE_SERVER, my_strlen (CONF_LINE_SERVER)))
        nsc_string_var (SET_CONF, CONF_SERVER_HOST, 
                        r + my_strlen (CONF_LINE_SERVER));

      if (!my_strncasecmp (r, CONF_LINE_LOGIN, my_strlen (CONF_LINE_LOGIN)))
        nsc_string_var (SET_CONF, CONF_LOGIN,
                        r + my_strlen (CONF_LINE_LOGIN));

      if (!my_strncasecmp (r, CONF_LINE_PORT, my_strlen (CONF_LINE_PORT)))
        nsc_int_var (SET_CONF, CONF_SERVER_PORT,
                     atoi (r + my_strlen (CONF_LINE_PORT)));

      if (!my_strncasecmp (r, CONF_LINE_PASSWD, my_strlen (CONF_LINE_PASSWD)))
        nsc_string_var (SET_CONF, CONF_PASS,
                        r + my_strlen (CONF_LINE_PASSWD));

      if (!my_strncasecmp (r, CONF_LINE_LOCATION, 
                           my_strlen (CONF_LINE_LOCATION)))
        nsc_string_var (SET_CONF, CONF_LOCATION, 
                        r + my_strlen (CONF_LINE_LOCATION));

      if (!my_strncasecmp (r, CONF_LINE_USERDATA,
                           my_strlen (CONF_LINE_USERDATA)))
        nsc_string_var (SET_CONF, CONF_USERDATA,
                        r + my_strlen (CONF_LINE_USERDATA));

      if (!my_strncasecmp (r, CONF_LINE_CONTACT,
                           my_strlen (CONF_LINE_CONTACT)))
        add_contact (r + my_strlen (CONF_LINE_CONTACT));

      if (!my_strncasecmp (r, CONF_LINE_IGNORE,
                           my_strlen (CONF_LINE_IGNORE)))
        add_ignore (r + my_strlen (CONF_LINE_IGNORE));

      if (!my_strncasecmp (r, CONF_LINE_AWAY_MSG,
                             my_strlen (CONF_LINE_AWAY_MSG)))
          nsc_string_var (SET_CONF, CONF_AUTO_MESSAGE_AWAY,
                          r + my_strlen (CONF_LINE_AWAY_MSG));

      if (!my_strncasecmp (r, CONF_LINE_LOCK_MSG,
                             my_strlen (CONF_LINE_LOCK_MSG)))
          nsc_string_var (SET_CONF, CONF_AUTO_MESSAGE_LOCK,
                          r + my_strlen (CONF_LINE_LOCK_MSG));

      if (!my_strncasecmp(r, CONF_LINE_SHOWSTATUSLIST, 
                          my_strlen(CONF_LINE_SHOWSTATUSLIST))) {
        nsc_int_var(SET_CONF, CONF_SHOWSTATUSLIST, 1);
      }

      if (!my_strncasecmp(r, CONF_LINE_SHOWSTATUSCHANGES,
                          my_strlen(CONF_LINE_SHOWSTATUSCHANGES))) {
        nsc_int_var(SET_CONF, CONF_SHOWSTATUSCHANGES, 1);
      }

      my_free (r);
      cont = 1;
    } /* if (r) */
  }
  while (cont);

  close (hand);
  my_free (file);

  printf_cl("Loading configuration done.\n");
  printf_cl ("You have %d friends.\n", num_contact_list());

  return (0);
}

int nsc_save_configuration(void)
{
  char *file = my_get_path_home (CONFIGURATION_FILE);
  FILE *hand;
  char *t;
  struct ignore_list_t *tign;

  hand = fopen(file, "w");
 
  if (hand == NULL) {
    printf_cl("Can't open %s\n", file);
    printf_cl("Saving configuration aborded\n");
    my_free(file);
    return (-1);
  }

  /* Ok, on sauve la configuration */

  fprintf(hand, "# Fichier genere automatiquement par nsc.\n");

  fprintf(hand, "%s%s\n", CONF_LINE_SERVER,
    nsc_string_var(GET_CONF, CONF_SERVER_HOST, NULL));

  fprintf(hand, "%s%d\n", CONF_LINE_PORT,
    nsc_int_var(GET_CONF, CONF_SERVER_PORT, 0));

  fprintf(hand, "%s%s\n", CONF_LINE_LOGIN,
    nsc_string_var(GET_CONF, CONF_LOGIN, NULL));

  fprintf(hand, "%s%s\n", CONF_LINE_PASSWD,
    nsc_string_var(GET_CONF, CONF_PASS, NULL));

  fprintf(hand, "%s%s\n", CONF_LINE_LOCATION,
    nsc_string_var(GET_CONF, CONF_LOCATION, NULL));

  fprintf(hand, "%s%s\n", CONF_LINE_USERDATA,
    nsc_string_var(GET_CONF, CONF_USERDATA, NULL));

  if (nsc_string_var(GET_CONF, CONF_AUTO_MESSAGE_AWAY, NULL) != NULL) {
    fprintf(hand, "%s%s\n", CONF_LINE_AWAY_MSG,
      nsc_string_var(GET_CONF, CONF_AUTO_MESSAGE_AWAY, NULL));
  } else {
    fprintf(hand, "# %sis not set\n", CONF_LINE_AWAY_MSG);
  }

  if (nsc_string_var(GET_CONF, CONF_AUTO_MESSAGE_LOCK, NULL) != NULL) {
    fprintf(hand, "%s%s\n", CONF_LINE_LOCK_MSG,
      nsc_string_var(GET_CONF, CONF_AUTO_MESSAGE_LOCK, NULL));
  } else {
    fprintf(hand, "# %sis not set\n", CONF_LINE_LOCK_MSG);
  }

  if (nsc_int_var(GET_CONF, CONF_SHOWSTATUSLIST, 0) == 1) {
    fprintf(hand, "%s\n", CONF_LINE_SHOWSTATUSLIST);
  } else {
    fprintf(hand, "# %s\n", CONF_LINE_SHOWSTATUSLIST);
  }

  if (nsc_int_var(GET_CONF, CONF_SHOWSTATUSCHANGES, 0) == 1) {
    fprintf(hand, "%s\n", CONF_LINE_SHOWSTATUSCHANGES);
  } else {
    fprintf(hand, "# %s\n", CONF_LINE_SHOWSTATUSCHANGES);
  }

  fprintf(hand, "\n# Contact list:\n");

  while ((t = get_next_contact_list_name())) {
    fprintf(hand, "contact %s\n", t);
  }

  fprintf(hand, "\n# Ignore list:\n");
  while ((tign = get_next_ignore_list_name())) {
    if (tign->message != NULL)
      fprintf(hand, "ignore %s %s\n", tign->name, tign->message);
    else
      fprintf(hand, "ignore %s\n", tign->name);
  }
  fclose(hand);
  my_free(file);

  printf_cl("Saving configuration done.\n");

  return (0);
}
