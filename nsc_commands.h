/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#ifndef HEADER_COMMAND
#define HEADER_COMMAND

typedef struct command_t
{
  char name[33];
  int (*cmd_handler)(char *buffer);
  char description[32];
  char completion;
} command, *pcommand;

extern command commands_list[];

pcommand get_command_struct (char *text);

int cmd_help (char *str);
int cmd_exit (char *str);
int cmd_reco (char *str);
int cmd_deco (char *str);
int cmd_connect (char *str);

int cmd_raw (char *str);
int cmd_user_message (char *str);
int cmd_user_state (char *str);
int cmd_user_whois (char *str);

int cmd_user_list (char *str);

int cmd_reload_config(char *str);
int cmd_save_config(char *str);

int cmd_add_contact(char *str);
int cmd_rem_contact(char *str);
int cmd_list_contact(char *str);

int cmd_add_ignore(char *str);
int cmd_rem_ignore(char *str);
int cmd_list_ignore(char *str);

int cmd_away(char *str);
int cmd_actif(char *str);

int cmd_version(char *str);
int cmd_query(char *str);

int cmd_userdata(char *str);
int cmd_location(char *str);

#define COM_NOCOMP	0x00
#define COM_NICK 	0x01
#define COM_STATE 	0x02

extern int count_whois;
extern int count_list;
extern int last_cmd;

#define CMD_WHOIS   0x10
#define CMD_LIST    0x11  

#endif
