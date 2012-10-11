/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#ifndef HEADER_GET_NEXT_LINE
#define HEADER_GET_NEXT_LINE

#define TAILLE_READ 128 

int is_there_get_next_data (void);
char *get_next_line(const int fd);

#endif
