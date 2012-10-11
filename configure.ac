# configure.ac -- process this file through Autoconf to produce 'configure'

AC_PREREQ([2.54])

AC_INIT([nsc], [0.1.6], [mycroft@virgaria.org])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([nsc_code.c])
AM_INIT_AUTOMAKE([foreign 1.6 dist-bzip2])
AC_CONFIG_HEADERS([config.h])

# We need the C compiler and utilities.
AC_PROG_CC

# This program has parameters. Set them depending on configure flags.

# Debugging.
AC_ARG_ENABLE([debug],
	      [AC_HELP_STRING([--enable-debug],
	                      [disable debugging stuff (default: no)])],
	      [ac_cv_debug=$enableval],
	      [ac_cv_debug=no])
if test x$ac_cv_debug != xno; then
   CPPFLAGS="$CPPFLAGS -DNETWORK_DEBUG"
fi

# Paths for EPITA.
AC_ARG_WITH([pie],
	    [AC_HELP_STRING([--with-pie],
	                    [add path information relative to EPITA (default: no)])],
	    [ac_cv_pie=$withval],
	    [ac_cv_pie=no])
if test x$ac_cv_pie != xno; then
   CPPFLAGS="$CPPFLAGS -I/usr/pkg/include -I/u/dp/public/arch/`uname -m`-`uname -s`/include"
   LDFLAGS="$LDFLAGS -L/usr/pkg/lib -L/u/dp/public/arch/`uname -m`-`uname -s`/lib"
fi

# SENDPING.
AC_ARG_WITH([ping-delay], 
	    [AC_HELP_STRING([--with-ping-delay=NNN],
	                    [keepalive timeout in seconds (default: 300)])],
	    [ac_cv_ping_delay=$withval],
	    [ac_cv_ping_delay=300])
AC_DEFINE_UNQUOTED([SENDPING], [$ac_cv_ping_delay],
	           [number of seconds between server pings])

# SELECTSEC.
AC_ARG_WITH([select-delay],
	    [AC_HELP_STRING([--with-select-delay=NNN],
	                    [select timeout in seconds (default: 60)])],
	    [ac_cv_select_delay=$withval],
	    [ac_cv_select_delay=60])
AC_DEFINE_UNQUOTED([SELECTSEC], [$ac_cv_select_delay], 
	           [number of seconds for select(2) timeout])

# AUTOCONNECT.
AC_ARG_ENABLE([auto-connect],
	      [AC_HELP_STRING([--disable-auto-connect],
	                      [disable auto-connect on startup (default: enable)])],
	      [ac_cv_auto_connect=$enableval],
	      [ac_cv_auto_connect=enable])
if test x$ac_cv_auto_connect != xdisable; then
  AC_DEFINE([AUTOCONNECT], [1], [define to enable auto-connect])
fi

# AUTORECONNECT.
AC_ARG_ENABLE([auto-reconnect],
	      [AC_HELP_STRING([--enable-auto-reconnect],
	                      [enable auto-reconnect on deconnection (default: yes)])],
	      [ac_cv_auto_reconnect=$enableval],
	      [ac_cv_auto_reconnect=yes])
if test x$ac_cv_auto_reconnect != xno; then
  AC_DEFINE([AUTORECONNECT], [1], [define to enable auto-reconnect])
fi

# Readline.
AC_WITH_READLINE
AC_WITH_HISTORY

# MD5 from OpenSSL.
AC_CHECK_HEADER([openssl/md5.h])
AC_SEARCH_LIBS([MD5], [crypto])

dnl # start of Libtool & ltdl stuff
dnl AC_ENABLE_STATIC(no)
dnl AC_ENABLE_SHARED(yes)

dnl AC_LIBLTDL_CONVENIENCE
dnl AC_SUBST(LTDLINCL)
dnl AC_SUBST(LIBLTDL)
dnl AC_CONFIG_SUBDIRS([libltdl])

dnl AC_WITH_LIBTOOL_FIX

dnl #end of libtool stuff
AC_PROG_RANLIB

# We might have use of the C++ compiler
AC_PROG_CXX


# Generate the configuration files.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
