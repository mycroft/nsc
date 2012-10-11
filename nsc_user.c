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
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <signal.h>

#include "my_readline.h"

#include "my_functions.h"
#include "nsc_functions.h"
#include "nsc_user.h"
#include "nsc_conf.h"
#include "nsc_main.h"
#include "nsc_commands.h"
#include "nsc_contact.h" 

static int moncodeestmoche = 0;

/* Completion handlers */

char *command_completion(char *text, int unused)
{
  char *name;
  static struct command_t *tmp = commands_list;

  while (tmp->cmd_handler)
    {
      if (!strncasecmp(text, tmp->name, strlen(text))) {
	name = strdup (tmp->name);
	tmp ++;
	return (name);
      }
      tmp ++ ;
    }
  
  tmp = commands_list; 

  return(NULL);
}

char *nick_completion(char *text, int unused)
{
  char *tmp;
  char *name;

  do {
    tmp = get_next_contact_list_name();

    if (tmp != NULL) {
      if (my_strncasecmp(text, tmp, strlen(text)) == 0) {
        name = strdup (tmp);
        return (name);
      }
    }

  } while (tmp != NULL);

  return (NULL);
}

#define STATUS_SIZE_ENTRY 32
static const char* const status_name[] = {
  "actif",
  "away",
  "server",
  "idle",
  "lock",
  NULL
};

char *state_completion(char *text, int unused)
{
  char *name;
  static const char * const*cur = status_name;

  while (*cur != NULL)
  {
    if (my_strncasecmp(text, *cur, strlen(*cur)) == 0) {
      name = strdup (*cur);
      cur ++;
      return (name);
    }
    cur ++;
  }

  cur = status_name;

  return (NULL);
}

char **complete(char *text, int start, int unused)
{
  char **matches = (char **)NULL;

  if (start != 0) {
    /* argument */
    matches = completion_matches(text, (CPFunction*)nick_completion);

    if (matches == (char **)NULL)
      matches = completion_matches(text, (CPFunction*)state_completion);
  }

  if (matches == (char **)NULL)
    matches = completion_matches(text, (CPFunction*)command_completion);

  return (matches);
}

void console_handler (char *str)
{
  pcommand tmp;
  char *cmd, *cmd_tmp;
  char *buffer, *q;

  if (str == NULL) {
    /* Ctrl+D */
    printf("\n");
    cmd_exit(str);
    return ;
  }

  cmd_tmp = str_to_first_word (str);
  cmd = strip_spaces (cmd_tmp);

  if (!cmd || my_strlen (cmd) == 0)
  {
    nsc_user_update ();
    my_free (cmd);
    return ;
  }

#ifdef HAVE_HISTORY
  add_history (str);
#endif
  moncodeestmoche = 1;

  for (tmp = commands_list; tmp->cmd_handler; tmp ++)
  {
    if (my_strcasecmp (tmp->name, cmd) == 0)
    {
      tmp->cmd_handler (str + my_strlen(cmd_tmp));

      /* returning. */
      goto end;
 
    }
  }

  /* ?! no command ? -> query !*/
  if ((q = nsc_string_var (GET_CONF, CNX_QUERY, NULL)) != NULL)
  {
    /* Send message to 'q' */
    buffer = malloc (my_strlen(q) + my_strlen (str) + 3);
    sprintf(buffer, " %s %s", q, str);
    cmd_user_message (buffer);
    my_free(buffer);
  } else {
    printf("No such command / No query.\n");
  }

end:
  my_free (cmd);
  my_free (cmd_tmp);

  moncodeestmoche = 0;

  nsc_user_update ();

  return ;
} 

int get_screen_size (int *x, int *y)
{
 struct winsize        w;

  if (ioctl(0, TIOCGWINSZ, &w) < 0)
    printf_cl ("ioctl: %s\n", strerror(errno));
  *x = w.ws_row;
  *y = w.ws_col;
  return 0;
}

void clear_line (void)
{
  char *blank_line = NULL;
  size_t len;
  int x, y;

  get_screen_size (&x, &y);

  if (rl_line_buffer == NULL)
    return ;
  /* Il prend bien la taille du buffer, mais il commence la ou il faut pas */
  len = my_strlen(rl_line_buffer) + my_strlen (rl_prompt); 

  if (blank_line)
    my_free(blank_line);
  blank_line = my_malloc (4096);
  my_memset (blank_line, 0, 4096);
  my_memset (blank_line, '\b', len);
  my_memset (blank_line + len, ' ', len);
  my_memset (blank_line + (len * 2), '\b', len);

  printf("%s", blank_line);
  my_free (blank_line);

  return ; 
}

int printf_error (const char *format, ...)
{
  va_list ap;

  va_start (ap, format);
  vfprintf (stderr, format, ap);
  va_end (ap);

  return (0);
}

int printf_normal (const char *format, ...)
{
  va_list ap;

  va_start (ap, format);
  vprintf (format, ap);
  va_end (ap);

  return (0);
}

int printf_cl (const char *format, ...)
{
  va_list ap;
  int save_point = rl_point;
  char buffer[1024];
  struct tm *t;
  time_t timed;

  if (!moncodeestmoche)
  {
    rl_end_of_line (0, 0);
    rl_redisplay ();
    clear_line ();
  }
  va_start (ap, format);
  vsnprintf (buffer, 1023, format, ap);
  va_end (ap);

  timed = time(NULL);
  t = localtime (&timed);
  printf ("%.2d:%.2d %s", t->tm_hour, t->tm_min, buffer);

  if (!moncodeestmoche)
  {
    rl_point = save_point;
    rl_forced_update_display();
  }

  return (0);
}

char *return_new_prompt (void)
{
  char *buffer, *q;

  if ((q = nsc_string_var (GET_CONF, CNX_QUERY, NULL)) != NULL)
  {
    buffer = my_malloc (
      sizeof (char) * (my_strlen(DEFAULT_PROMPT) + my_strlen(q) + 2));
    sprintf (buffer, "%s/%s", q, DEFAULT_PROMPT); 
  }
  else
  {
    buffer = my_malloc (sizeof (char) * (my_strlen(DEFAULT_PROMPT) + 1));
    sprintf (buffer, "%s", DEFAULT_PROMPT);
  }

  return (buffer);
}


int nsc_user_update (void)
{
  char *prompt = NULL;

  prompt = return_new_prompt ();
  rl_callback_handler_remove ();
  rl_callback_handler_install (prompt, console_handler);

  if(prompt != NULL) {
    my_free(prompt);
  }

  return 0;
}

int nsc_user_init (void)
{
  char *prompt = NULL;
  struct sigaction act;

  rl_catch_sigwinch = 1;

  act.sa_handler = sigwinch_handler;
  sigemptyset(&(act.sa_mask));
  act.sa_flags = 0;
  sigaction(SIGWINCH, &act, NULL);

  prompt = return_new_prompt (); 
  rl_filename_completion_desired = 0;
  rl_attempted_completion_function = (CPPFunction*)complete;
  rl_callback_handler_install (prompt, console_handler);

  if(prompt != NULL) {
    my_free(prompt);
    prompt = NULL;
  }
  return (0);
}

int nsc_user_close (void)
{
  if (rl_prompt != NULL)
    my_free (rl_prompt);

  rl_prompt = NULL;
  rl_callback_handler_remove ();
  return 0;
}
