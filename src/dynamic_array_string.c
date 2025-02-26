#include "../libft/includes/libft.h"
#include "../includes/dynamic_array_string.h"

/* typedef struct s_dyn
{
    char **list;  // Tableau de chaînes de caractères
    int length;   // Nombre d'éléments actuels
    int capacity; // Capacité maximale actuelle
} t_dyn; */

void init_dyn(t_dyn *list)
{
    list->capacity = 2;  // Capacité initiale
    list->length = 0;
    list->list = malloc(list->capacity * sizeof(char *));
    if (!list->list)
    {
        ft_printf("Erreur d'allocation mémoire\n");
        exit(1);
    }
}

void resize_dyn(t_dyn *list)
{
    int new_capacity = list->capacity * 2;
    char **new_list = malloc(new_capacity * sizeof(char *));
    if (!new_list)
    {
        ft_printf("Erreur d'allocation mémoire (resize)\n");
        exit(1);
    }

    // Copier les anciens éléments
    for (int i = 0; i < list->length; i++)
        new_list[i] = list->list[i];

    free(list->list);  // Libérer l'ancien tableau
    list->list = new_list;
    list->capacity = new_capacity;
}

void append(t_dyn *list, const char *item)
{
    // Si on atteint la capacité, on agrandit
    if (list->length >= list->capacity)
        resize_dyn(list);
    
    // Allocation et copie de la chaîne
    //list->list[list->length] = malloc(ft_strlen(item) + 1);
    list->list[list->length] = ft_strdup(item); 
    if (!list->list[list->length])
    {
        ft_printf("Erreur d'allocation pour la chaîne\n");
        exit(1);
    }
    //strcpy(list->list[list->length], item);
    list->length++;
}

void free_dyn(t_dyn *list)
{
    for (int i = 0; i < list->length; i++)
        free(list->list[i]);  // Libération des chaînes
    free(list->list);         // Libération du tableau
}

/* int main(void)
{
    t_dyn list;
    init_dyn(&list);

    append(&list, "Hello");
    append(&list, "World");
    append(&list, "Dynamic");
    append(&list, "Array");

    for (int i = 0; i < list.length; i++)
        printf("%s\n", list.list[i]);

    free_dyn(&list);
    return 0;
} */