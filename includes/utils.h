#ifndef UTILS_H
# define UTILS_H
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include "./types.h"
#include "../includes/utils.h"
#include "../libft/includes/libft.h"

char	*ft_strjoin_multiple(const char *first, ...);
void	*ft_memcpy_fast(void *dest, const void *source, size_t n);
void 	*ft_memcpy_faster(void *dest, const void *src, size_t n);
t_fileData		*malloc_fileData(void);
void	clean_path(char *path);

#endif