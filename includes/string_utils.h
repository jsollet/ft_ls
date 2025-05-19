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
void build_lookup_table(const char *set, bool table[CHAR_RANGE]);

ssize_t find_first_of(const char *str, const char *set, size_t start);
ssize_t find_last_of(const char *str, const char *set, size_t start);
ssize_t find_first_not_of(const char *str, const char *set, size_t start);
ssize_t find_last_not_of(const char *str, const char *set, size_t start);
char *ft_strnchr(const char *str, int c, size_t end);
char *ft_strndup_range(const char *s, size_t start, size_t end);
bool    is_in_set(char c, const char *set);
bool is_in_set_fast(char c, const bool table[CHAR_RANGE]);
bool   starts_with(const char *str, const char *prefix); // c++20
bool starts_with_case_insensitive(const char *str, const char *prefix);
bool ends_with(const char *str, const char *suffix);
bool ends_with_case_insensitive(const char *str, const char *suffix);
char *ft_strchrnul(const char *s, int c);
size_t ft_strcspn(const char *s, const char *reject);
size_t ft_strspn(const char *s, const char *accept);
bool contains(const char *haystack, const char *needle);
bool contains_case_insensitive(const char *haystack, const char *needle);
size_t skip_while(const char *s, const char *set, size_t start);
size_t skip_until(const char *s, const char *set, size_t start);
size_t count_char(const char *str, char c);
char *ft_strtrim_custom(const char *str, const char *set);
char *ft_strtok_custom(const char *s, const char *delims, size_t *start);


char*   find_format_block(const char *str,  size_t *start, t_format_block *position);
char*   find_text_block(const char *str, size_t *start, t_format_block *position);











// Optionnelles (non implémentées mais déclarées pour l'avenir) voir avec gpt
// https://chatgpt.com/c/68015bdc-cb3c-8011-86a2-67e1b9f6a1b4
bool    starts_with_n(const char *str, const char *prefix, size_t n);
bool    ends_with_n(const char *str, const char *suffix, size_t n);
bool    starts_with_n_ci(const char *str, const char *prefix, size_t n);
bool    ends_with_n_ci(const char *str, const char *suffix, size_t n);
#endif