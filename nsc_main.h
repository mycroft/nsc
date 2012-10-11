/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#ifndef HEADER_NSC_MAIN
#define HEADER_NSC_MAIN

#define NSC_ASK_AUTH "auth_ag ext_user none none\n"
#define NSC_AUTH_ANS "ext_user_log %s %s %s %s\n"

#include "ymca.h"

int main_lc (void);
int main_cc (void);
int main_prepare_exit (void);
void sigwinch_handler(int sig); 

volatile int propagate_sigwinch;
user_ymca people;
int fin;

#endif /* HEADER_NSC_MAIN */
