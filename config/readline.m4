# This file contains Autoconf macros to detect libreadline
# properly.

AC_DEFUN([AC_WITH_READLINE], [dnl
    AC_CACHE_CHECK([for readline library],
	           [ac_cv_lib_readline],
	           [dnl
        ac_cv_lib_readline=''
	old_LIBS=$LIBS
        for readline_lib in readline edit editline; do
          for termcap_lib in "" termcap curses ncurses; do
            if test -z "$termcap_lib"; then
              TRY_LIB="-l$readline_lib"
            else
              TRY_LIB="-l$readline_lib -l$termcap_lib"
            fi
            LIBS="$ORIG_LIBS $TRY_LIB"
            AC_LINK_IFELSE([AC_LANG_PROGRAM([], [readline(0)])],
	                   [ac_cv_lib_readline="$TRY_LIB"], [])
            if test -n "$ac_cv_lib_readline"; then
              break
            fi
          done
          if test -n "$ac_cv_lib_readline"; then
            break
          fi
        done
        if test -z "$ac_cv_lib_readline"; then
          ac_cv_lib_readline="no"
        fi
        LIBS=$old_LIBS
    ])
    if test x"$ac_cv_lib_readline" != xno; then
       LIBS="$LIBS $ac_cv_lib_readline"
       AC_DEFINE([HAVE_LIBREADLINE], [1],
	         [define if you have a readline compatible library])
       AC_CHECK_HEADERS(readline.h readline/readline.h)
    fi
])

AC_DEFUN([AC_WITH_HISTORY], [dnl
  AC_REQUIRE([AC_WITH_READLINE])

  ac_cv_have_history=no
  if test x"$ac_cv_lib_readline" != xno; then
     AC_CACHE_CHECK([whether readline supports history],
	            [ac_cv_readline_history],
                    [dnl
       AC_LINK_IFELSE([AC_LANG_PROGRAM([], [add_history(0)])],
	              [ac_cv_readline_history=yes], 
	              [ac_cv_readline_history=no])
     ])
     if test x"$ac_cv_readline_history" != xno; then
       ac_cv_have_history=yes
       AC_DEFINE([HAVE_READLINE_HISTORY], [1],
                 [define if your readline library has 'add_history'])
     else
       AC_CACHE_CHECK([for history library],
	              [ac_cv_lib_history],
	              [dnl
            old_LIBS="$LIBS"
            LIBS="$LIBS -lhistory"
            AC_LINK_IFELSE([AC_LANG_PROGRAM([], [add_history(0)])],
	                   [ac_cv_lib_history=-lhistory],
	                   [ac_cv_lib_history=no])
	    LIBS=$old_LIBS
       ])
       if test x"$ac_cv_lib_history" != xno; then
	  ac_cv_have_history=yes
	  LIBS="$LIBS $ac_cv_lib_history"
          AC_DEFINE([HAVE_LIBHISTORY], [1],
                    [define if you have a history library besides readline])
       fi
     fi
  else
     AC_MSG_WARN([no readline library available, cannot use history])
  fi
  if test x"$ac_cv_have_history" != xno; then
    AC_DEFINE([HAVE_HISTORY], [1],
	      [define if history is available])
    AC_CHECK_HEADERS(history.h readline/history.h)
  fi
])


