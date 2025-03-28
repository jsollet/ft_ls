#include "../libft/includes/libft.h"
#include "../includes/dynamic_array_string.h"



void init_dyn(t_dyn *list)
{
    list->capacity = 2;  // Capacité initiale
    list->length = 0;
    list->total_size = 0;
    //list->list = malloc(list->capacity * sizeof(char *));
    list->list = malloc(list->capacity * sizeof(t_fileData *));
    if (!list->list)
    {
        ft_printf_fd(2,"Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }
}

void resize_dyn(t_dyn *list)
{
    int new_capacity = list->capacity * 2;
    //char **new_list = malloc(new_capacity * sizeof(char *));
    t_fileData **new_list =  malloc(new_capacity * sizeof(t_fileData *));
    if (!new_list)
    {
        ft_printf_fd(2,"Erreur d'allocation mémoire (resize)\n");
        exit(EXIT_FAILURE);
    }

    // Copier les anciens éléments
    for (int i = 0; i < list->length; i++)
        new_list[i] = list->list[i];

    free(list->list);  // Libérer l'ancien tableau
    list->list = new_list;
    list->capacity = new_capacity;
}

void append(t_dyn *list, t_fileData *item)
{
    // Si on atteint la capacité, on agrandit
    if (list->length >= list->capacity)
        resize_dyn(list);
    
    // Allocation et copie de la chaîne
    //list->list[list->length] = malloc(ft_strlen(item) + 1);
    list->list[list->length] = item; 
    if (!list->list[list->length])
    {
        ft_printf_fd(2,"Erreur d'allocation pour la chaîne\n");
        exit(EXIT_FAILURE);
    }
    //strcpy(list->list[list->length], item);
    list->length++;
}

void free_dyn(t_dyn *list)
{
    for (int i = 0; i < list->length; i++){
        if (list->list[i]->owner){free(list->list[i]->owner);}
        if (list->list[i]->group){free(list->list[i]->group);}
        if (list->list[i]->fileName){free(list->list[i]->fileName);}
        if (list->list[i]->absolutePath){free(list->list[i]->absolutePath);}
        if (list->list[i]->path){free(list->list[i]->path);}
        if (list->list[i]->link_target){free(list->list[i]->link_target);}
        
        free(list->list[i]);// Libération des items
        }  
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