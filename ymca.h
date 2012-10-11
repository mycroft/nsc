/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#ifndef _YMCA_
#define _YMCA_

#define E_ERROR -1
#define E_TIMEOUT -2

#include <sys/time.h>

typedef struct ymca_t
{
  struct ymca_t *next;
  int fd;
  void *ptr_data;
} *ymca;

typedef struct user_ymca_t
{
  struct ymca_t *ymca;
  struct ymca_t *ready_fd;
} *user_ymca;

/* create a YMCA structure */
user_ymca uy_create_socket_list (void);

/* destroy socket list closes fd, but doesn t free data ptrs */
int destroy_socket_list (user_ymca *socket_liste);

/* Api interne */
int is_fd_in_socket_list (ymca socket_liste, int fd);
int get_max_fd_value_from_socket_list (ymca socket_liste);

/* add a fd to a list, and return new 'ymca' cell */
ymca add_fd_to_socket_list (user_ymca *socket_liste, int fd);
ymca add_fd_to_ymca_list (ymca *sl, int fd);
ymca add_fd_data_to_socket_list (user_ymca *socket_liste, int fd, void *data);

/* have a look in socket list and return data. */
void *get_data_from_socket_list_with_fd (user_ymca socket_liste, int fd);

/* remove a fd from list, unallow memory, and return next structure */
/* doesn't remove if from ready_fds */
ymca remove_fd_from_socket_list (user_ymca *socket_liste, int fd, void **data);


void *get_data_and_remove_fd_from_socket_list (ymca *socket_liste, int fd);
int get_next_active_fd (user_ymca socket_liste, struct timeval *timeout);

#endif
