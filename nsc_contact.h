/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#ifndef HEADER_CONTACT
#define HEADER_CONTACT

typedef struct contact_list_t {
  char *name;
} contact_list_cel;

typedef struct ignore_list_t {
  char *name;
  char *message;
} ignore_list_cel;

#include "listes.h"

int add_contact (char *name);
int add_ignore (char *name);
int rem_contact (char *name);
int rem_ignore (char *name);
int flush_contact_list (void);
int flush_ignore_list (void);
cel *search_first_cel_with_name_in_list(char *name, pcel liste);
char *get_next_contact_list_name (void);
char *build_accolades_contact_list (void);
int num_contact_list(void);
struct ignore_list_t *get_next_ignore_list_name(void);
struct ignore_list_t *is_ignore(char *name);

#endif
