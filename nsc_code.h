/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#ifndef HEADER_NSC_CODE
#define HEADER_NSC_CODE

#define BACK_SLASH_N '\n'
#define CHAR_URI_ESC '%'
#define BACK_SLASH '\\'

int ns_back_slash_decode(char *buff);
int ns_url_decode(unsigned char *buff);
char *ns_url_encode(unsigned char *buff);
char *ns_back_slash_encode(char *buff);
char *ns_encode(char *buff);
char *ns_decode(char *buff);

#endif
