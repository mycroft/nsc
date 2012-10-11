/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#ifndef HEADER_NSC_CONF
#define HEADER_NSC_CONF

#define CONFIGURATION_FILE ".nscrc"

#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 4096
#endif

#define DEFAULT_LOCATION "viva.nsc.forever"
#define DEFAULT_USERDATA "nsc, c'est trop de la balle"

#define SET_CONF 1
#define GET_CONF 2

/* Data that ll be handled by nsc_load_configuration and
                              nsc_free_configuration */
#define CONF_SERVER_HOST 1
#define CONF_SERVER_PORT 2
#define CONF_LOGIN       3
#define CONF_PASS        4
#define CONF_LOCATION    5
#define CONF_USERDATA    6

/* Data that ll be handled in runtime */

#define CNX_CLIENT_FD    7
#define CNX_CLIENT_HOST  8
#define CNX_CLIENT_PORT  9
#define CNX_SESSION_ID   10 
#define CNX_CHAL_STR     11
#define CNX_STATUS       12
#define CNX_SERVER_TS    13
#define CNX_QUERY        14

#define CONF_AUTO_MESSAGE_AWAY	15
#define CONF_AUTO_MESSAGE_LOCK	16

#define CNX_TYPE	17

#define CONF_SHOWSTATUSLIST	18
#define CONF_SHOWSTATUSCHANGES	19

#define TYPE_INTERNE	0x01
#define TYPE_EXTERNE	0x02


/* Different client status */

#define STATUS_NOT_CONNECTED  0
#define STATUS_NOT_AUTH       1
#define STATUS_AUTH_SENT      2
#define STATUS_ATTACH_SENT    3
#define STATUS_CONNECTION     4
#define STATUS_ACTIF          5 
#define STATUS_AWAY           6
#define STATUS_IDLE           7 
#define STATUS_LOCK           8

int nsc_int_var(char action, char nom, int value);
char *nsc_string_var(char action, char nom, char *value);

int nsc_free_configuration(void);
int nsc_load_configuration(void);
int nsc_save_configuration(void);

#endif /* HEADER_NSC_CONF */
