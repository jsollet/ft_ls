#ifndef ATTR_ACL_H
# define ATTR_ACL_H
#include "../libft/includes/libft.h"
#include "./types.h"
#include "./error.h"
#include "./utils.h"
#include <unistd.h>
#include <errno.h>
#define EXPORT __attribute__((visibility("default")))
#if defined(__APPLE__)
# include <sys/xattr.h>
# include <sys/acl.h>
# else
#  include "../local/install/include/attr/libattr.h"
#  include "../local/install/include/acl/libacl.h"
#endif

char    has_xattr(const char *path, t_exit_status *exit_status);
char    has_acl(const char *path, char **text, t_exit_status *exit_status);
char    *format_acl_text(const char *acl_text);
char 	*format_acl_text_linux(const char *acl_text);
void    get_xattr(t_fileData *file, t_exit_status *exit_status);
#endif