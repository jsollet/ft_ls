#ifndef SORT_H
# define SORT_H

#include <stdlib.h>
#include "types.h"
#include "../libft/includes/libftprintf_fd.h"

int     compare_by_fileName(t_fileData *a, t_fileData *b);
int     compare_by_time(t_fileData *a, t_fileData *b);
int		compare_by_atime(t_fileData *a, t_fileData *b);
int     compare_by_fileName_reverse(t_fileData *a, t_fileData *b);
int     compare_by_time_reverse(t_fileData *a, t_fileData *b);
int 	compare_by_atime_reverse(t_fileData *a, t_fileData *b);
int		(*get_cmp_func(t_flags *flags))(t_fileData *, t_fileData *) ;
void    mergeSort_iterative(t_fileData *arr[], int n,int (*cmp)(t_fileData *, t_fileData *) );

#endif