/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <mycroft@virgaria.org> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return. Patrick MARIE
 * ----------------------------------------------------------------------------
 */

#ifndef HEADER_MY_FUNCTION
#define HEADER_MY_FUNCTION

#include <sys/types.h>

#ifdef DEBUG
#define my_malloc(memory) \
    my_function_malloc ((memory), __FILE__, __func__, __LINE__)

#define my_free(memory) \
    my_function_free ((memory), __FILE__, __func__, __LINE__)

/* Don't use these one. Use my_malloc and my_free */
void *my_function_malloc (size_t size, char *s, char *f, int i);
void my_function_free (void *buf, char *s, char *f, int i);
#else
void *my_malloc (size_t size);
void my_free (void *buf);
#endif

void my_memory_repport (void);
void *my_realloc (void *ptr, size_t oldsize, size_t newsize);

void *my_memcpy(void *dst, const void *src, size_t len);
void *my_memset(void *b, int c, size_t len);

int my_strlen(char *str);
int my_strcmp (const char *s1, const char *s2);
int my_strncmp (const char *s1, const char *s2, size_t len);
int my_strcasecmp (const char *s1, const char *s2);
int my_strncasecmp (const char *s1, const char *s2, size_t n);
char *my_strcpy(char *dst, const char *src);
char *my_strcat(char *s, const char *append);

char *my_malloc_strcpy (char *str);
char *malloc_copie_cat (char *str1, char *str2);

char *my_get_path_home (char *file);
char *strip_spaces (char *str);
char *str_to_first_word (char *str);
char **my_str_to_wordtab (char * str , char sep);

int my_tolower (int c);

ssize_t my_read(int fd, void *buf, size_t count);
ssize_t my_write(int fd, const void *buf, size_t count);

#endif /* HEADER_MY_FUNCTION */
