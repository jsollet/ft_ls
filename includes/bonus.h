#ifndef BONUS_H
# define BONUS_H
#include "../libft/includes/libft.h"
#include <sys/xattr.h>
#include <sys/acl.h>
#include <unistd.h>
#include <errno.h>

char has_xattr(const char *path);
char has_acl(const char *path, char **text);
char *format_acl_text(const char *acl_text);

#endif