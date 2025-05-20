#ifndef STRING_UTILS_H
# define  STRING_UTILS_H
#include "./allheaders.h"
#include "../libft/includes/libft.h"

typedef struct s_format_block {
    size_t start;
    size_t end;
    size_t length;
    bool   is_format;
} t_format_block;


#define CHAR_RANGE 256
int ft_strcmp(const char *s1, const char *s2);
ssize_t find_first_of(const char *str, const char *set, size_t start);
size_t skip_while(const char *s, const char *set, size_t start);
char*   find_format_block(const char *str,  size_t *start, t_format_block *position);
char*   find_text_block(const char *str, size_t *start, t_format_block *position);
#endif