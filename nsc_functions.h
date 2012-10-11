/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#ifndef HEADER_NSC_FUNCTIONS
#define HEADER_NSC_FUNCTIONS

#include <sys/types.h>

ssize_t writebuffer (int sock, char *fmt, ...);
ssize_t writebuffer_no_fmt (int sock, char *str);

#endif /* HEADER_NSC_FUNCTIONS */
