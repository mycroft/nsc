/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

#include "ymca.h"

user_ymca uy_create_socket_list (void)
{
  user_ymca new;
  new = (user_ymca)my_malloc(sizeof(struct user_ymca_t));
  new->ymca = NULL;
  new->ready_fd = NULL;

  return (new);
}

int vider_struct_ymca_t (ymca elt)
{
  if (elt->fd)
    close (elt->fd);

  free (elt);

  return (0);
}

int destroy_socket_list (user_ymca *socket_liste)
{
  ymca tmp_ptr = (*socket_liste)->ymca;
  ymca tmp_ptr_next;

  while (tmp_ptr)
  {
    tmp_ptr_next = tmp_ptr->next;

    /* warning: This doesn't free ->data */
    vider_struct_ymca_t (tmp_ptr);

    tmp_ptr = tmp_ptr_next;
  }

  tmp_ptr = (*socket_liste)->ready_fd;
  while (tmp_ptr)
  {
    tmp_ptr_next = tmp_ptr->next;
    vider_struct_ymca_t (tmp_ptr);
    tmp_ptr = tmp_ptr_next;
  }

  free(*socket_liste);
  *socket_liste = NULL;

  return (0);
}

int is_fd_in_socket_list (ymca socket_liste, int fd)
{
  ymca tmp_ptr = socket_liste;

  while (tmp_ptr)
  {
    if (tmp_ptr->fd == fd)
     return (1);
    tmp_ptr = tmp_ptr->next;
  }

  return (0);
}

int get_max_fd_value_from_socket_list (ymca socket_liste)
{
  int max = -1;
  ymca tmp_ptr = socket_liste;

  while (tmp_ptr) 
  {
    if (max < tmp_ptr->fd)
      max = tmp_ptr->fd;
    tmp_ptr = tmp_ptr->next;
  }

  return (max);
}

ymca add_fd_to_socket_list (user_ymca *socket_liste, int fd)
{
  ymca new = NULL;

  if (!is_fd_in_socket_list ((*socket_liste)->ymca, fd))
  {
    if ((new = malloc (sizeof (struct ymca_t) * 1)))
    {
      new->next = (*socket_liste)->ymca;
      new->ptr_data = NULL;
      new->fd = fd;
      (*socket_liste)->ymca = new;
    }
  }

  return (new);
}

ymca add_fd_to_ymca_list (ymca *sl, int fd)
{
  ymca new = NULL;

  if (!is_fd_in_socket_list (*sl, fd))
  {
    if ((new = malloc (sizeof (struct ymca_t) * 1)))
    {
      new->next = *sl;
      new->ptr_data = NULL;
      new->fd = fd;
      *sl = new;
    }
  } 

  return (new);
}

ymca add_fd_data_to_socket_list (user_ymca *socket_liste, int fd, void *data)
{
  ymca new = NULL;

  if ((new = add_fd_to_socket_list (socket_liste, fd)))
    new->ptr_data = data;

  return (new);
}

void *get_data_from_socket_list_with_fd (user_ymca socket_liste, int fd)
{
  ymca tmp_ptr = socket_liste->ymca;
  void *data = NULL;

  while (tmp_ptr)
  {
    if (tmp_ptr->fd == fd)
    {
      data = tmp_ptr->ptr_data; 
      break;
    }

    tmp_ptr = tmp_ptr->ptr_data;
  }

  return (data);
}

ymca remove_fd_from_socket_list (user_ymca *socket_liste, int fd, void **data)
{
  ymca tmp_ptr = (*socket_liste)->ymca;
  ymca av_tmp_ptr = NULL;
  ymca n = NULL;

  while (tmp_ptr)
  {
    if (tmp_ptr->fd == fd)
    {
      n = tmp_ptr->next;

      if (data)
        *data = tmp_ptr->ptr_data;

      if (!av_tmp_ptr)
      {
        (*socket_liste)->ymca = tmp_ptr->next;
        vider_struct_ymca_t (tmp_ptr);
      }
      else
      {
        av_tmp_ptr->next = tmp_ptr->next;
        vider_struct_ymca_t (tmp_ptr);
      }

      break ;
    }

    av_tmp_ptr = tmp_ptr;
    tmp_ptr = tmp_ptr->next;
  }

  return (n);
}

int get_next_active_fd (user_ymca socket_liste, struct timeval *timeout)
{
  fd_set sfuck;
  ymca tmp_ptr;
  int f_ret = -1;
  int ret;

  if (!(socket_liste->ready_fd))
  {
    FD_ZERO (&sfuck);

    tmp_ptr = socket_liste->ymca;
    while (tmp_ptr)
    {
      FD_SET (tmp_ptr->fd, &sfuck);
      tmp_ptr = tmp_ptr->next;
    }

    ret = select (get_max_fd_value_from_socket_list (socket_liste->ymca) + 1, 
                  &sfuck, NULL, NULL, timeout);

    switch (ret)
    {
      case -1:
	      /* if we were interrupted it could be because of SIGSTOP/SIGCONT,
	       * in which case a timeout is very likely to have occured.
	       * Reporting a timeout is therefore the safer bet. */
	if (errno == EINTR || errno == EAGAIN)
	   return E_TIMEOUT;
	return (E_ERROR);
      break;
      case 0:
        /* timeout */
        return(E_TIMEOUT);
      break;
      default:
        tmp_ptr = socket_liste->ymca;
        while (tmp_ptr)
        {
          if (FD_ISSET (tmp_ptr->fd, &sfuck))
            add_fd_to_ymca_list(&(socket_liste->ready_fd), tmp_ptr->fd);
          tmp_ptr = tmp_ptr->next;
        }
      break;
    }
  }

  if(socket_liste->ready_fd != NULL)
  {
    f_ret = socket_liste->ready_fd->fd;
    tmp_ptr = socket_liste->ready_fd->next;
    free (socket_liste->ready_fd);
    socket_liste->ready_fd = tmp_ptr;
  }

  return (f_ret);
}
