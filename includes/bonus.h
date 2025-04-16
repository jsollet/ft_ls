#ifndef BONUS_H
# define BONUS_H
#include "../libft/includes/libft.h"
#include "./types.h"
#include "./error.h"
#include <sys/xattr.h>
#include <sys/acl.h>
#include <unistd.h>
#include <errno.h>

char    has_xattr(const char *path, t_exit_status *exit_status);
char    has_acl(const char *path, char **text, t_exit_status *exit_status);
char    *format_acl_text(const char *acl_text);
void    get_xattr(t_fileData *file, t_exit_status *exit_status);
char	*ft_strjoin_multiple(const char *first, ...);
#endif