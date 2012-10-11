/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdarg.h>
#include <openssl/md5.h>

#include "my_functions.h"
#include "nsc_main.h"
#include "nsc_conf.h"
#include "nsc_code.h"
#include "nsc_user.h"

char *pt (unsigned char *md)
{
  int i;
  char *retstr = my_malloc (sizeof (char) * (MD5_DIGEST_LENGTH * 2 + 1));

  my_memset (retstr, 0, (MD5_DIGEST_LENGTH * 2 + 1));

  for (i = 0; i < MD5_DIGEST_LENGTH; i ++)
    sprintf(retstr + (2 * i), "%02x", md[i]);

  return (retstr);
}

char *build_auth_message (void)
{
  char *tmp_chalenge;
  char *tmp_client_host;
  unsigned short tmp_client_port;
  char *tmp_password;
  char *tmp_login;
  char *tmp_location, *tmp_location_enc;
  char *tmp_userdata, *tmp_userdate_enc;
  unsigned char pass[MD5_DIGEST_LENGTH];
  unsigned char buffer[MAX_BUFFER_SIZE];
  char *auth_message;
  char *md5buf;

  tmp_chalenge = nsc_string_var (GET_CONF, CNX_CHAL_STR, NULL);
  tmp_client_host = nsc_string_var (GET_CONF, CNX_CLIENT_HOST, NULL);
  tmp_password = nsc_string_var (GET_CONF, CONF_PASS, NULL);
  tmp_client_port = nsc_int_var (GET_CONF, CNX_CLIENT_PORT, 0);
  tmp_login = nsc_string_var (GET_CONF, CONF_LOGIN, NULL);
  tmp_location = nsc_string_var (GET_CONF, CONF_LOCATION, NULL);
  tmp_userdata = nsc_string_var (GET_CONF, CONF_USERDATA, NULL);

  snprintf((char*)buffer, MAX_BUFFER_SIZE - 1,
                   "%s-%s/%i%s",
                   tmp_chalenge, tmp_client_host, 
                   tmp_client_port, tmp_password);
  MD5(buffer, my_strlen((char*)buffer), pass);

  md5buf = pt(pass);

  if (tmp_location != NULL)
    tmp_location_enc = ns_encode (tmp_location);
  else
    tmp_location_enc = ns_encode (DEFAULT_LOCATION);

  if (tmp_userdata != NULL)
    tmp_userdate_enc = ns_encode (tmp_userdata);
  else
    tmp_userdate_enc = ns_encode (DEFAULT_USERDATA);

  auth_message = my_malloc (
     sizeof(char) * (my_strlen (NSC_AUTH_ANS) +
     my_strlen (tmp_login) + my_strlen (md5buf) + 
     my_strlen (tmp_userdate_enc) + my_strlen(tmp_location_enc)));

  sprintf (auth_message, NSC_AUTH_ANS, tmp_login, md5buf,
           tmp_location_enc, tmp_userdate_enc);

  my_free (tmp_location_enc);
  my_free (tmp_userdate_enc);
  my_free (md5buf);

  return (auth_message);
}
