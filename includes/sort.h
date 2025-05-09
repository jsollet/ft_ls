#ifndef SORT_H
# define SORT_H

#include <stdlib.h>
#include "types.h"
#include "../libft/includes/libftprintf_fd.h"

time_t  get_mtime(t_fileData *f);
time_t  get_atime(t_fileData *f);
int     cmp_mtime(t_fileData *a, t_fileData *b);
int		cmp_atime(t_fileData *a, t_fileData *b);
int		cmp_mtime_rev(t_fileData *a, t_fileData *b);
int		cmp_atime_rev(t_fileData *a, t_fileData *b);
int     compare_by_time_generic(t_fileData *a, t_fileData *b, get_time_func get_time);
int     compare_by_time_generic_reverse(t_fileData *a, t_fileData *b, get_time_func get_time);
int     compare_by_fileName(t_fileData *a, t_fileData *b);
int     compare_by_absolutePath(t_fileData *a, t_fileData *b);
int     compare_by_time(t_fileData *a, t_fileData *b);
int     compare_by_fileName_reverse(t_fileData *a, t_fileData *b);
int     compare_by_absolutePath_reverse(t_fileData *a, t_fileData *b);
int     compare_by_time_reverse(t_fileData *a, t_fileData *b);

int		(*get_cmp_func(t_flags *flags))(t_fileData *, t_fileData *) ;

void    mergeSort_iterative(t_fileData *arr[], int n,int (*cmp)(t_fileData *, t_fileData *) );

#endif