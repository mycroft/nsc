/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#ifndef HEADER_NSC_NETWORK
#define HEADER_NSC_NETWORK

struct in_addr resolv(char *hostname);
int nsc_connect_socket (void);
int nsc_close_socket (void);

#endif /* HEADER_NSC_NETWORK */

