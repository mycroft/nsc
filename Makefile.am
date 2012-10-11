AM_CFLAGS = -D_GNU_SOURCE

bin_PROGRAMS = nsc

lib_LIBRARIES = libnscutil.a

nsc_SOURCES = nsc_main.c my_functions.c ymca.c nsc_network.c nsc_conf.c	      \
     nsc_user.c get_next_line.c nsc_connect.c nsc_parser_hello.c	      \
     nsc_code.c nsc_auth.c nsc_functions.c nsc_parser_ping.c		      \
     nsc_commands.c nsc_parser_misc.c listes.c nsc_contact.c		      \
     listes.h nsc_auth.h  nsc_commands.h     nsc_connect.h		      \
     nsc_functions.h nsc_network.h nsc_user.h get_next_line.h  my_functions.h \
     nsc_code.h   nsc_conf.h  nsc_contact.h  nsc_main.h  nsc_parsers.h	      \
     ymca.h my_readline.h

libnscutil_a_SOURCES = nscutil_value.cc nsc_value.hh nsc_value.hxx \
	nscutil_conf.cc nsc_conf.hh nsc_conf.hxx

# SUBDIRS = libltdl

CLEANFILES = *core *\~ 
MAINTAINERCLEANFILES = aclocal.m4 config.h.in configure stamp-h.in \
	depcomp install-sh Makefile.in missing mkinstalldirs 


EXTRA_DIST = netsoul_spec.txt \
	config/readline.m4 config/libtool-fix.m4 \
	config/cxxwrap-sh.in config/ltwrap-sh.in

ACLOCAL_AMFLAGS = -I config
CONFIGURE_DEPENDENCIES = $(srcdir)/config/readline.m4 $(srcdir)/config/libtool-fix.m4


