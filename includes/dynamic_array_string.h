#ifndef DYNAMIC_ARRAY_STRING_H
# define DYNAMIC_ARRAY_STRING_H
#include "types.h"
#include "../libft/includes/libftprintf_fd.h"

typedef struct s_dyn
{
    t_fileData **list;
    int     length;
    int     capacity;
    long    total_size;
} t_dyn;

void    init_dyn(t_dyn *list);
void    resize_dyn(t_dyn *list);
void    append(t_dyn *list, t_fileData *item);
void    free_fileData(t_fileData *file);
void    free_dyn(t_dyn *list);
void    reset_dyn(t_dyn *dyn);
#endif