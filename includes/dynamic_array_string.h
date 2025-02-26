#ifndef DYNAMIC_ARRAY_STRING_H
# define DYNAMIC_ARRAY_STRING_H

typedef struct s_dyn
{
    char **list;  // Tableau de chaînes de caractères
    int length;   // Nombre d'éléments actuels
    int capacity; // Capacité maximale actuelle
} t_dyn;

void init_dyn(t_dyn *list);
void resize_dyn(t_dyn *list);
void append(t_dyn *list, const char *item);
void free_dyn(t_dyn *list);
#endif