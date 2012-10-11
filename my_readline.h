/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <raph@rhs.se> wrote this file. As long as you retain this notice you can
 * do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Raphael Poss
 * ----------------------------------------------------------------------------
 */

#ifndef MY_READLINE_H
#define MY_READLINE_H

#include "config.h"

#ifdef HAVE_LIBREADLINE
#  if defined(HAVE_READLINE_READLINE_H)
#    include <readline/readline.h>
#  elif defined(HAVE_READLINE_H)
#    include <readline.h>
#  else /* !defined(HAVE_READLINE_H) */
#    error Readline headers not found.
#  endif /* !defined(HAVE_READLINE_H) */

#else /* !defined(HAVE_READLINE_READLINE_H) */
#  error Readline not available.
#endif /* HAVE_LIBREADLINE */

#ifdef HAVE_HISTORY
#    if defined(HAVE_READLINE_HISTORY_H)
#       include <readline/history.h>
#    elif defined(HAVE_HISTORY_H)
#       include <history.h>
#    else /* !defined(HAVE_HISTORY_H) */
extern void add_history (const char *);
extern int write_history (const char *);
extern int read_history (const char *);
#   endif /* defined(HAVE_READLINE_HISTORY_H) */
#endif

#ifdef RL_VERSION_MAJOR
#  ifdef RL_VERSION_MINOR
#  if RL_VERSION_MAJOR >= 4
#    if RL_VERSION_MINOR >= 3
extern char **completion_matches PARAMS((char *, rl_compentry_func_t *));
#    endif
#  endif
#  endif
#endif

#endif
