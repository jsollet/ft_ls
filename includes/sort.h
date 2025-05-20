#ifndef SORT_H
# define SORT_H

#include <stdlib.h>
#include "types.h"
#include "../libft/includes/libftprintf_fd.h"

time_t  get_mtime(t_fileData *f);
time_t  get_atime(t_fileData *f);

int     compare_by_fileName(t_fileData *a, t_fileData *b);
int     compare_by_time(t_fileData *a, t_fileData *b);
int     compare_by_fileName_reverse(t_fileData *a, t_fileData *b);
int     compare_by_time_reverse(t_fileData *a, t_fileData *b);
int		(*get_cmp_func(t_flags *flags))(t_fileData *, t_fileData *) ;
void    mergeSort_iterative(t_fileData *arr[], int n,int (*cmp)(t_fileData *, t_fileData *) );

#endif