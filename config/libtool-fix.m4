
AC_DEFUN([AC_WITH_LIBTOOL_FIX],
[dnl
 AC_REQUIRE([AC_PROG_CC])
 AC_REQUIRE([AC_PROG_LD])

 AC_LIBTOOL_DLOPEN

 dnl The old way (2.13) of doing things, which libtool expects:
 dnl     AC_PROVIDE(AC_LIBTOOL_DLOPEN)
 dnl make it explicit:
 define([AC_PROVIDE_AC_LIBTOOL_DLOPEN], )

 AC_PROG_LIBTOOL

# Because of a bug in libtool's policy for linking shared
# libraries and modules, in most case a wrapper is needed.
# However a flag is needed to disable it on some platforms.
AC_ARG_ENABLE(ltwrap,
              AC_HELP_STRING([--enable-ltwrap],
                             [enable fixing libtool for C++ links]),
              [if test $enableval = yes; then
                  AC_MSG_NOTICE([using ltwrap.sh for calls to libtool])
               else
                  AC_MSG_WARN([not using ltwrap.sh for calls to libtool])
               fi
               cppsh_cv_enable_ltwrap=$enableval],
              AC_CACHE_CHECK([whether to use ltwrap.sh],
                             [cppsh_cv_enable_ltwrap], 
                             [cppsh_cv_enable_ltwrap=no]))

if test x$cppsh_cv_enable_ltwrap = xyes; then
   LIBTOOL='$(SHELL) $(top_builddir)/ltwrap.sh $(top_builddir)'
   AC_CONFIG_FILES([cxxwrap.sh:$am_aux_dir/cxxwrap-sh.in], [chmod +x cxxwrap.sh])
   AC_CONFIG_FILES([ltwrap.sh:$am_aux_dir/ltwrap-sh.in], [chmod +x ltwrap.sh])

   # fix the libtool script by removing the definition of CC:
   AC_MSG_NOTICE([fixing the libtool script for use with ltwrap.sh])
   cat libtool | sed 's,^CC=.*,,g' >libtool.tmp && mv -f libtool.tmp libtool
   chmod +x libtool
fi
])


