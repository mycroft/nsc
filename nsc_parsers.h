/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#ifndef HEADER_NSC_PARSERS
#define HEADER_NSC_PARSERS

/* This file ll 've all parsers functions */

int parse_hello (char *buffer);
int parse_validity (char *buf);
int parse_ping (char *buffer);
int send_ping (int sec);
int parse_user_cmd (char *buffer);
int parse_user_list (char *buffer);
int parse_cmd_end (char *buffer);

#endif
