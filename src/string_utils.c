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

void build_lookup_table(const char *set, bool table[CHAR_RANGE]) {
    memset(table, 0, CHAR_RANGE); // Initialise tout à false
    if (set == NULL)
        return;
    for (size_t i = 0; set[i]; ++i)
        table[(unsigned char)set[i]] = true;
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


ssize_t find_last_of(const char *str, const char *set, size_t start) {
    if (!str  || !set || ft_strlen(str) <= start)
        return -1;
    for (ssize_t i = (ssize_t)start; i >= 0; --i) {
        for (size_t j = 0; set[j]; ++j) {
            if (str[i] == set[j])
                return i;
        }
    }
    return -1;
}


ssize_t find_first_not_of(const char *str, const char *set, size_t start) {
    if ( !str  || !set  || ft_strlen(str) < start )
        return -1;
    for (size_t i = start; str[i]; ++i) {
        int found = 0;
        for (size_t j = 0; set[j]; ++j) {
            if (str[i] == set[j]) {
                found = 1;
                break;
            }
        }
        if (!found)
            return i;
    }
    return -1;
}

ssize_t find_last_not_of(const char *str, const char *set, size_t start) {
    if (!str  || !set || ft_strlen(str) <= start)
        return -1;
    for (ssize_t i = (ssize_t)start; i >= 0; --i) {
        int found = 0;
        for (size_t j = 0; set[j]; ++j) {
            if (str[i] == set[j]) {
                found = 1;
                break;
            }
        }
        if (!found)
            return i;
    }
    return -1;
}

char *ft_strnchr(const char *str, int c, size_t end) {
    if (!str )
        return NULL;

    for (size_t i = 0; i < end && str[i]; ++i) {
        if (str[i] == (char)c)
            return (char *)(str + i);
    }

    return NULL;
}

char *ft_strndup_range(const char *str, size_t start, size_t end){
    // version avec bornes ouvertes ([start, end))
    if (!str || start > end || ft_strlen(str) <= end)
        return NULL;

    size_t len = end - start; // +1 avec borne fermee
    char *dest;

    dest = malloc((len + 1) * sizeof(char));
    if (!dest)
        return NULL;
    ft_memcpy(dest, str + start,len);
    dest[len] = '\0';
    //printf("[\n%s\n]", dest);
    return dest;
}

bool    is_in_set(char c, const char *set){
    if (!set)
        return false;
    for (size_t j = 0; set[j]; ++j) {
        if (c == set[j])
            return true;
    }
    return false;
}

bool is_in_set_fast(char c, const bool table[CHAR_RANGE]) {
    return table[(unsigned char)c];
}

bool   starts_with(const char *str, const char *prefix){
    if (!str || !prefix)
        return false;
    size_t i = 0;
    while (prefix[i]) {
        if (str[i] != prefix[i])
            return false;
        i++;
    }
    return true;
}

bool starts_with_case_insensitive(const char *str, const char *prefix) {
    if (!str || !prefix)
        return false;

    size_t i = 0;
    while (prefix[i]) {
        char a = str[i];
        char b = prefix[i];

        // On convertit en minuscule sans utiliser ctype.h
        if (a >= 'A' && a <= 'Z')
            a += 32;
        if (b >= 'A' && b <= 'Z')
            b += 32;

        if (a != b)
            return false;
        i++;
    }
    return true;
}

bool ends_with(const char *str, const char *suffix) {
    if (!str || !suffix)
        return false;

    size_t len_str = ft_strlen(str);
    size_t len_suf = ft_strlen(suffix);

    if (len_suf > len_str)
        return false;

    return ft_strcmp(str + len_str - len_suf, suffix) == 0;
}

bool ends_with_case_insensitive(const char *str, const char *suffix) {
    if (!str || !suffix)
        return false;

    size_t len_str = strlen(str);
    size_t len_suf = strlen(suffix);

    if (len_suf > len_str)
        return false;

    const char *sub = str + len_str - len_suf;

    for (size_t i = 0; i < len_suf; ++i) {
        char a = sub[i];
        char b = suffix[i];

        if (a >= 'A' && a <= 'Z')
            a += 32;
        if (b >= 'A' && b <= 'Z')
            b += 32;

        if (a != b)
            return false;
    }
    return true;
}

// a voir si c' est utile !
char *ft_strchrnul(const char *s, int c) {
    while (*s) {
        if (*s == (char)c)
            return (char *)s;
        s++;
    }
    return (char *)s; // Pointeur sur le '\0'
}

size_t ft_strcspn(const char *s, const char *reject) {
    size_t i = 0;

    while (s[i]) {
        for (size_t j = 0; reject[j]; j++) {
            if (s[i] == reject[j])
                return i;
        }
        i++;
    }
    return i;
}

size_t ft_strspn(const char *s, const char *accept) {
    size_t i = 0;

    while (s[i]) {
        size_t j = 0;
        while (accept[j] && s[i] != accept[j])
            j++;
        if (!accept[j])
            break;
        i++;
    }
    return i;
}

bool contains(const char *haystack, const char *needle) {
	if (!haystack || !needle)
		return false;
	return ft_strnstr(haystack, needle, ft_strlen(haystack)) != NULL;
}


static int strncasecmp_ascii(const char *s1, const char *s2, size_t n) {
	while (n--) {
		unsigned char c1 = (unsigned char)*s1++;
		unsigned char c2 = (unsigned char)*s2++;
		c1 = ft_tolower(c1);
		c2 = ft_tolower(c2);
		if (c1 != c2)
			return (c1 - c2);
		if (c1 == '\0')
			break;
	}
	return 0;
}

bool contains_case_insensitive(const char *haystack, const char *needle) {
	if (!haystack || !needle)
		return false;

	size_t hay_len = strlen(haystack);
	size_t needle_len = strlen(needle);

	if (needle_len == 0)
		return true;
	if (hay_len < needle_len)
		return false;

	for (size_t i = 0; i <= hay_len - needle_len; ++i) {
		if (strncasecmp_ascii(haystack + i, needle, needle_len) == 0)
			return true;
	}
	return false;
}

size_t skip_while(const char *str, const char *set, size_t start){
    if (!str || !set || ft_strlen(str) <= start)
        return start;
    
    if (*set == '\0')  // Si set est vide, on renvoie simplement start
        return start;
    size_t i = start;

    while (str[i])
    {
        size_t j = 0;
        while (set[j] && str[i] != set[j])
            j++;
        if (!set[j]) // pas trouvé dans set
            break;
        i++;
    }
    return i;
}

size_t skip_until(const char *str, const char *set, size_t start){
    if (!str || !set || ft_strlen(str) <= start)
        return start;
    size_t i = start;

    if (*set == '\0')  // Si set est vide, on renvoie simplement start
        return start;

    while (str[i])
    {
        size_t j = 0;
        while (set[j] && str[i] != set[j])
            j++;
        if (!set[j]){ // pas trouvé dans set
            i++;
            continue;
        } else {
            break;
        }
        
    }
    return i;
}

size_t count_char(const char *str, char c) {
    size_t count = 0;
    if (!str)
        return 0;
    while (*str) {
        if (*str == c)
            count++;
        str++;
    }
    return count;
}

char *ft_strtrim_custom(const char *str, const char *set) {
    size_t start = 0;
    size_t end;

    if (!str || !set)
        return NULL;

    end = ft_strlen(str);
    while (str[start] && ft_strchr(set, str[start]))
        start++;
    while (end > start && ft_strchr(set, str[end - 1]))
        end--;

    char *trimmed = malloc(end - start + 1);
    if (!trimmed)
        return NULL;

    ft_memcpy(trimmed, str + start, end - start);
    trimmed[end - start] = '\0';
    return trimmed;
}

char *ft_strtok_custom(const char *s, const char *delims, size_t *start) {
    if (!s || !delims || !start || s[*start] == '\0')
        return NULL;

    size_t i = *start;

    // Skip leading delimiters
    while (s[i] && ft_strchr(delims, s[i]))
        i++;

    if (s[i] == '\0') {
        *start = i;
        return NULL; // No more tokens
    }

    size_t token_start = i;

    // Move forward until we find a delimiter or reach end of string
    while (s[i] && !ft_strchr(delims, s[i]))
        i++;

    size_t token_len = i - token_start;
    char *token = malloc(token_len + 1);
    if (!token)
        return NULL;

    ft_memcpy(token, s + token_start, token_len);
    token[token_len] = '\0';

    *start = i; // Update position to next scan point
    return token;
}

// 


// Optionnelles (non implémentées mais déclarées pour l'avenir) voir avec gpt
// https://chatgpt.com/c/68015bdc-cb3c-8011-86a2-67e1b9f6a1b4
bool starts_with_ci(const char *str, const char *prefix) {
    if (!str || !prefix)
        return false;
    while (*prefix) {
        if (ft_tolower((unsigned char)*str) != ft_tolower((unsigned char)*prefix))
            return false;
        ++str;
        ++prefix;
    }
    return true;
}

bool ends_with_ci(const char *str, const char *suffix) {
    if (!str || !suffix)
        return false;

    size_t len_str = strlen(str);
    size_t len_suffix = strlen(suffix);

    if (len_suffix > len_str)
        return false;

    str += len_str - len_suffix;
    while (*suffix) {
        if (tolower((unsigned char)*str) != tolower((unsigned char)*suffix))
            return false;
        ++str;
        ++suffix;
    }
    return true;
}


char* find_format_block(const char *str, size_t *start, t_format_block *position) {
    if (!str || !start || str[*start] == '\0')
        return NULL;

    ssize_t next_percent = find_first_of(str, "%", *start);
    if (next_percent == -1 || str[next_percent + 1] == '\0')
        return NULL;

    // chercher un vrai caractère de conversion après le %
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
    *start = end + 1; // mise à jour de start pour la suite

    return format_block;
}


// foire, non c' est les test qui foirait...
char* find_text_block(const char *str, size_t *start, t_format_block *position) {
    if (!str || !start || str[*start] == '\0')
        return NULL;

    ssize_t next_percent = find_first_of(str, "%", *start);
    size_t len;

    if (next_percent == -1) {
        len = strlen(str + *start); //ok
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


