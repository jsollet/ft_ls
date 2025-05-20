#include "../includes/string_utils.h"
#include "../includes/buffer.h"

int ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

ssize_t find_first_of(const char *str, const char *set, size_t start) {
    if (!str  || !set || ft_strlen(str) < start)
        return -1;
    for (size_t i = start; str[i]; ++i) {
        for (size_t j = 0; set[j]; ++j) {
            if (str[i] == set[j])
                return (ssize_t)i;
        }
    }
    return -1;
}


size_t skip_while(const char *str, const char *set, size_t start){
    if (!str || !set || ft_strlen(str) <= start)
        return start;
    
    if (*set == '\0')
        return start;
    size_t i = start;

    while (str[i])
    {
        size_t j = 0;
        while (set[j] && str[i] != set[j])
            j++;
        if (!set[j])
            break;
        i++;
    }
    return i;
}


char* find_format_block(const char *str, size_t *start, t_format_block *position) {
    if (!str || !start || str[*start] == '\0')
        return NULL;

    ssize_t next_percent = find_first_of(str, "%", *start);
    if (next_percent == -1 || str[next_percent + 1] == '\0')
        return NULL;

    ssize_t end = find_first_of(str, "cspdiuxX%", next_percent + 1);
    if (end == -1)
        return NULL;

    size_t len = end - next_percent + 1;
    char *format_block = ft_substr(str, next_percent, len);
    if (!format_block)
        return NULL;

    position->start = next_percent;
    position->end = end;
    position->is_format = true;
    position->length = end - next_percent + 1;
    *start = end + 1;

    return format_block;
}



char* find_text_block(const char *str, size_t *start, t_format_block *position) {
    if (!str || !start || str[*start] == '\0')
        return NULL;

    ssize_t next_percent = find_first_of(str, "%", *start);
    size_t len;

    if (next_percent == -1) {
        len = ft_strlen(str + *start); //ok
    } else {
        len = next_percent - *start;
    }

    if (len == 0)
        return NULL;

    char *text_block = ft_substr(str, *start, len); // ok
    if (!text_block)
        return NULL;
    
    position->start = *start;
    position->end = *start + len - 1;
    position->is_format = false;
    position->length = len;
    *start += len;

    return text_block;
}