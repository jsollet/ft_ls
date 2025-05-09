#ifndef UTILS_H
# define UTILS_H
#include <stdlib.h>
#include "./types.h"
#include "../includes/utils.h"
#include "../libft/includes/libft.h"


void	*ft_memcpy_fast(void *dest, const void *source, size_t n);
void 	*ft_memcpy_faster(void *dest, const void *src, size_t n);
t_fileData		*malloc_fileData(void);
void	clean_path(char *path);

#endif