
#include <stdio.h> 
#include <sys/xattr.h>
#include <sys/acl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include "../includes/bonus.h"

char	has_xattr(const char *path)
{
	#ifdef __APPLE__
		if (listxattr(path, NULL, 0, XATTR_NOFOLLOW) > 0)
			return '@';
	#else
		if (listxattr(path, NULL, 0) > 0)
			return '@';
	#endif
		return ' ';
}

char	has_acl(const char *path, char **text )
{
	acl_t acl = NULL;
	#ifdef __APPLE__
		acl = acl_get_file(path, ACL_TYPE_EXTENDED);
		if (acl == NULL) {
			return ' ';
		}
	#else
		acl = acl_get_file(path, ACL_TYPE_ACCESS);
	#endif
		if (acl) {
			*text = acl_to_text(acl, NULL);
			acl_free(acl);
			return '+';
		}
		return ' ';
}


char	*ft_strjoin_multiple(const char *first, ...){
	if (!first)
		return NULL;
	size_t total_length = 0;
	const char *current_str = first;
	va_list args;
	va_start(args, first);
	// Calculer la longueur totale de toutes les chaînes
	while (current_str) {
		total_length += ft_strlen(current_str);
		current_str = va_arg(args, const char *);
	}
	va_end(args);
	// Alloue la mémoire pour la chaîne finale
	char *result = malloc(total_length + 1);
	if (!result)
		return NULL;
	
	va_start(args, first);
	char *current_pos = result;
	current_str = first;
	while (current_str) {
		size_t len = ft_strlen(current_str);
		ft_memcpy(current_pos, current_str, len);
		current_pos += len;
		current_str = va_arg(args, const char *);
	}
	va_end(args);
	
	*current_pos = '\0';  // Termine la chaîne par un '\0'
	return result;
}

char *check_and_free(char **tokens, char *line) {
    if (!line) {
        free(tokens);
        return NULL;
    }
    return line;
}

char *concat_with_newline(char *result, const char *line) {
    char *newline = NULL;
    if (*result)
        newline = ft_strjoin(result, "\n");
    else
        newline = ft_strdup(""); // Pas de saut de ligne si result est vide

    char *new_result = ft_strjoin(newline, line);
    free(newline);
	free(result);
    return new_result;
}

char *format_acl_text(const char *acl_text) {
	char *result = NULL;
	char **lines = NULL;

	if (!acl_text || !(lines = ft_split(acl_text, '\n')) || !(result = ft_strdup(""))) {
    	free(lines);
    	return NULL;
	}

	for (int i = 1; lines[i]; i++) {// i = 1 avant
		char **tokens = ft_split(lines[i], ':');
		if (!tokens)
			continue;

		// On s'assure qu'on a au moins 6 tokens
		if (tokens[0] && tokens[2] && tokens[4] && tokens[5]) {
			char *index = ft_itoa(i-1);
			// Utilisation de la fonction variadique sans avoir besoin de spécifier le nombre d'arguments
			char *line = ft_strjoin_multiple(index, ":", tokens[0], ":", tokens[2], " ", tokens[4], " ", tokens[5], NULL);
			if (!check_and_free(tokens, line))
    			continue;

			result = concat_with_newline(result, line);
			free(line);
			free(index);
		}

		// Free les tokens
		for (int j = 0; tokens[j]; j++)
			free(tokens[j]);
		free(tokens);
	}

	// Free les lignes
	for (int i = 0; lines[i]; i++)
		free(lines[i]);
	free(lines);

	return result;
}



char	*format_acl_text1(const char *acl_text) {
	if (!acl_text)
		return NULL;

	char **lines = ft_split(acl_text, '\n');
	if (!lines)
		return NULL;

	char *result = ft_strdup("");
	if (!result)
		return NULL;

	for (int i = 1; lines[i]; i++) {
		char **tokens = ft_split(lines[i], ':');
		if (!tokens)
			continue;

		// on s'assure qu'on a au moins 6 tokens (user, UUID, name, ID, allow, perms)
		if (tokens[0] && tokens[2] && tokens[4] && tokens[5]) {
			char *line = ft_strdup("");  // ligne formatée temporaire
			char *tmp;

			tmp = ft_strjoin(line, tokens[0]);  // user
			free(line);
			line = tmp;

			tmp = ft_strjoin(line, ":");
			free(line);
			line = tmp;

			tmp = ft_strjoin(line, tokens[2]);  // root
			free(line);
			line = tmp;

			tmp = ft_strjoin(line, " ");
			free(line);
			line = tmp;

			tmp = ft_strjoin(line, tokens[4]);  // allow
			free(line);
			line = tmp;

			tmp = ft_strjoin(line, " ");
			free(line);
			line = tmp;

			tmp = ft_strjoin(line, tokens[5]);  // read
			free(line);
			line = tmp;

			tmp = ft_strjoin(result, *result ? "\n" : "");  // saut de ligne si nécessaire
			free(result);
			result = tmp;

			tmp = ft_strjoin(result, line);  // ajout au résultat
			free(result);
			result = tmp;

			free(line);
		}

		// free les tokens
		for (int j = 0; tokens[j]; j++)
			free(tokens[j]);
		free(tokens);
	}

	// free les lignes
	for (int i = 0; lines[i]; i++)
		free(lines[i]);
	free(lines);

	return result;
}