/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#ifndef HEADER_NSC_USER
#define HEADER_NSC_USER

#define DEFAULT_PROMPT "nsc# "
#define PRINT_ERROR_SYNTAX "[!] "

void console_handler (char *str);
int get_screen_size (int *x, int *y);
void clear_line (void);

int printf_error (const char *format, ...);
int printf_normal (const char *format, ...);
int printf_cl (const char *format, ...);

char *return_new_prompt (void);

int nsc_user_init (void);
int nsc_user_update (void);
int nsc_user_close (void);

#endif /* HEADER_NSC_USER */ 
