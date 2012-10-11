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
#include <sys/time.h>
#include <unistd.h>

#include <signal.h>

#include "my_readline.h"

#include "config.h"

#include "my_functions.h"
#include "nsc_conf.h"
#include "nsc_user.h"
#include "nsc_connect.h"
#include "nsc_network.h"
#include "ymca.h"
#include "nsc_code.h"
#include "nsc_main.h"
#include "nsc_parsers.h"

int print_intro(void)
{
  printf(
"    _   __     __  _____             ___________            __          ___\n" 
"   / | / /__  / /_/ ___/____  __  __/ / ____/ (_)__  ____  / /_   _   _|__ \\\n"
"  /  |/ / _ \\/ __/\\__ \\/ __ \\/ / / / / /   / / / _ \\/ __ \\/ __/  | | / /_/ /\n"
" / /|  /  __/ /_ ___/ / /_/ / /_/ / / /___/ / /  __/ / / / /_    | |/ / __/ \n"
"/_/ |_/\\___/\\__//____/\\____/\\__,_/_/\\____/_/_/\\___/_/ /_/\\__/    |___/____/ \n");

  return 0;
}

int main_lc (void)
{
  int n = 0;

  if (nsc_int_var (GET_CONF, CNX_STATUS, 0) != STATUS_NOT_CONNECTED)
  {
    printf_cl ("Connection asked while already connected ... aborting.\n");
    return (-1);
  }

  printf_cl ("Connecting to netsoul ...\n");

#ifdef AUTOCONNECT
  do
  {
    if (n)
    {
      printf_cl ("Error connecting to netsoul ... Retrying.\n"); 
      printf_cl ("Waiting 60 seconds before retrying\n"); 
      sleep (60);
    }
#endif
    n = nsc_connect_socket ();
    if (n > 0)
      add_fd_to_socket_list (&people, n);
#ifdef AUTOCONNECT
  } while (n == -1);
#endif

  return (0);
}

int main_cc (void)
{
  int n;

  n = nsc_close_socket ();
  if (n > 0) {
    printf_cl ("Closing connection to netsoul ...\n");
    remove_fd_from_socket_list (&people, n, NULL);
  }

  nsc_int_var (SET_CONF, CNX_STATUS, STATUS_NOT_CONNECTED);

  return (0);
}

int main_prepare_exit (void)
{
  fin = 1;
  
  return (0);
}

void sigwinch_handler(int sig) 
{
	propagate_sigwinch = sig; 
//	rl_cleanup_after_signal();
}

int main (void)
{
  struct timeval ts;
  int ret_sel;

  print_intro ();

  printf ("nsc - version 2 - %s - get last release on "
          "http://etud.epita.fr/~mycroft/\n\n", VERSION);

  people = uy_create_socket_list ();
  fin = 0;

  /* Initialisation */
  nsc_user_init ();
  nsc_load_configuration ();

  add_fd_to_socket_list (&people, 0);

#ifdef AUTOCONNECT
  main_lc ();
#endif

  do
  {
    ts.tv_usec = 0;
#ifdef SELECTSEC
    ts.tv_sec = SELECTSEC;
#else
	ts.tv_sec = 60;
#endif

    ret_sel = get_next_active_fd (people, &ts);

    switch (ret_sel)
    {
      case E_TIMEOUT:
        if (send_ping (ts.tv_sec) == -1)
        {
          main_cc ();
#ifdef AUTORECONNECT
          main_lc ();
#endif
          continue ;
        }
      break;
      case E_ERROR:
        if (propagate_sigwinch != 0) {
          propagate_sigwinch = 0;
          continue;
        }
        printf_cl("Fatal error.");
        fin = 1;

      break;
      case 0:
        /* User command line data */
        rl_callback_read_char();
      break;
      default: /* Different of 0, so it ll not be used for user entry */
        if ((ret_sel == nsc_int_var (GET_CONF, CNX_CLIENT_FD, 0)) > 0)
        {
          if ((nsc_recv_data () == -1))
          { /* There is some troubles with the NS server */
            /* Going to shutdown the connection */
            main_cc ();
#ifdef AUTORECONNECT
            main_lc ();
#endif
            continue ;
          }
        }
      break;
    } 
  }
  while (!fin);

  main_cc ();

  printf_cl ("Exiting nsc. Thanks using me !\n");

  nsc_save_configuration ();
  clear_line ();

  nsc_free_configuration ();
  nsc_user_close ();

  destroy_socket_list (&people);

#ifdef DEBUG
  my_memory_repport ();
#endif

  return 0;
}
