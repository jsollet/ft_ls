#ifndef DYNAMIC_ARRAY_STRING_H
# define DYNAMIC_ARRAY_STRING_H
#include "types.h"
#include "../libft/includes/libftprintf_fd.h"

typedef struct s_dyn
{
    //char **list;  // Tableau de chaînes de caractères
    t_fileData **list;
    int     length;   // Nombre d'éléments actuels
    int     capacity; // Capacité maximale actuelle
    long    total_size; // La taille totale des fichiers
} t_dyn;

void init_dyn(t_dyn *list);
void resize_dyn(t_dyn *list);
void append(t_dyn *list, t_fileData *item);
void free_dyn(t_dyn *list);
#endif