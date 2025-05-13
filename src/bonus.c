
#include <stdio.h> 
#include <sys/xattr.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include "../includes/bonus.h"
#include "../includes/types.h"
#include "../libft/includes/libftprintf.h"


ssize_t	get_xattr_size(t_fileData *file, char *buffer, ssize_t size){
	#ifdef __APPLE__
		size = listxattr(file->absolutePath, buffer, size, XATTR_NOFOLLOW);
	#else
		size = listxattr(file->absolutePath, buffer, size);
	#endif
	return size;
}

void	fill_xattr_structure(t_fileData *file, char *buffer, ssize_t size){
	int	j = 0;
	int	i = 0;
	
	while (i < size){

		char *name = &buffer[i];
		file->xattrs[j].name = ft_strdup(name);
		#ifdef __APPLE__
		file->xattrs[j].size = getxattr(file->absolutePath, name, NULL, 0, 0, XATTR_NOFOLLOW);
		#else
		file->xattrs[j].size = getxattr(file->absolutePath, name, NULL, 0);
		#endif
		i += ft_strlen(name) + 1;
		j++;
	}
	
}

void 	get_xattr(t_fileData *file,  t_exit_status *exit_status){
	ssize_t	size = get_xattr_size(file, NULL, 0);
	ssize_t tmp;
	
	char	*buffer = NULL;
	int		count = 0;
	int		i = 0;



	if (size < 0) {
		set_exit_status(exit_status, 1, "NULL");
        return;
    }

	if (size == 0) {
        return;
    }

	buffer = malloc(size); // 
	if (!buffer){
		set_exit_status(exit_status, 1, "NULL");
		return;
	}
	tmp = get_xattr_size(file, buffer, size);
	if (tmp < 0) {
        free(buffer);
		set_exit_status(exit_status, 1, "NULL");
        return;
    }

	while (i < size){
		i += ft_strlen(&buffer[i]) + 1;
		count++;
	}
	file->xattrs = malloc(sizeof(t_attr) * count);
	if (!file->xattrs)
	{
		free(buffer);
		set_exit_status(exit_status, 1, "NULL");
		return ;
	}
	for (int j = 0; j < count; j++) {
        file->xattrs[j].name = NULL;
    }
	file->xattr_count = count;
	fill_xattr_structure(file, buffer, size);
	free(buffer);
}


char has_xattr(const char *path, t_exit_status *exit_status)
{
    #ifdef __APPLE__
        ssize_t result = listxattr(path, NULL, 0, XATTR_NOFOLLOW);
        if (result > 0)
            return '@';
        else if (result == 0)
            return ' ';
        else {
			set_exit_status(exit_status, 1, strerror(errno));
            return ' ';  
        }
    #else
        ssize_t result = listxattr(path, NULL, 0);
        if (result > 0)
            return '@';
        else if (result == 0)
            return ' ';
        else {
			set_exit_status(exit_status, 1, strerror(errno));
            return ' ';
        }
    #endif
}
#ifdef __APPLE__
char	has_acl(const char *path, char **text, t_exit_status *exit_status)
{
	acl_t acl = NULL;
	#ifdef __APPLE__
		acl = acl_get_file(path, ACL_TYPE_EXTENDED);
		if (acl == NULL) {
			set_exit_status(exit_status, 1, strerror(errno));
			return ' ';
		}
	#else
		acl = acl_get_file(path, ACL_TYPE_ACCESS);
		if (acl == NULL) {
			set_exit_status(exit_status, 1, strerror(errno));
			return ' ';
		}
	#endif
		if (acl) {
			*text = acl_to_text(acl, NULL);
			acl_free(acl);
			return '+';
		}
		return ' ';
}
#endif

char	*ft_strjoin_multiple(const char *first, ...){
	if (!first)
		return NULL;
	size_t total_length = 0;
	const char *current_str = first;
	va_list args;
	va_start(args, first);
	while (current_str) {
		total_length += ft_strlen(current_str);
		current_str = va_arg(args, const char *);
	}
	va_end(args);

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
	
	*current_pos = '\0';
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
		newline = ft_strdup("");

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

	for (int i = 1; lines[i]; i++) {
		char **tokens = ft_split(lines[i], ':');
		if (!tokens)
			continue;

		if (tokens[0] && tokens[2] && tokens[4] && tokens[5]) {
			char *index = ft_itoa(i-1);			
			char *line = ft_strjoin_multiple(index, ":", tokens[0], ":", tokens[2], " ", tokens[4], " ", tokens[5], NULL);
			if (!check_and_free(tokens, line))
				continue;
			result = concat_with_newline(result, line);
			free(line);
			free(index);
		}
		for (int j = 0; tokens[j]; j++)
			free(tokens[j]);
		free(tokens);
	}
	for (int i = 0; lines[i]; i++)
		free(lines[i]);
	free(lines);
	return result;
}