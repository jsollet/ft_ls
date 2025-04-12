#include "../libft/includes/libft.h"
#include "../includes/dynamic_array_string.h"

void    init_dyn(t_dyn *list)
{
	list->capacity = 2;
	list->length = 0;
	list->total_size = 0;
	list->list = malloc(list->capacity * sizeof(t_fileData *));
	if (!list->list)
	{
		ft_printf_fd(2,"Erreur d'allocation mémoire\n");
		exit(EXIT_FAILURE);
	}
}

void    resize_dyn(t_dyn *list)
{
	int new_capacity = list->capacity * 2;
	t_fileData **new_list =  malloc(new_capacity * sizeof(t_fileData *));
	if (!new_list)
	{
		ft_printf_fd(2,"Erreur d'allocation mémoire (resize)\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < list->length; i++)
		new_list[i] = list->list[i];

	free(list->list);
	list->list = new_list;
	list->capacity = new_capacity;
}

void    append(t_dyn *list, t_fileData *item)
{
	if (list->length >= list->capacity)
		resize_dyn(list);
	list->list[list->length] = item; 
	if (!list->list[list->length])
	{
		ft_printf_fd(2,"Erreur d'allocation pour la chaîne\n");
		exit(EXIT_FAILURE);
	}
	list->length++;
}

void free_fileData(t_fileData *file)
{
	if (!file ) // attention || !file->valid
		return;
	if (file->owner) free(file->owner);
	if (file->group) free(file->group);
	if (file->fileName) free(file->fileName);
	if (file->absolutePath) free(file->absolutePath);
	if (file->path) free(file->path);
	if (file->link_target) free(file->link_target);
	if (file->acl_text) free(file->acl_text);
	if (file->xattrs  && file->has_xattr == '@') { // a tester
		for (int j = 0; j < file->xattr_count; j++) {
			if (file->xattrs[j].name){
				free(file->xattrs[j].name);
			}
		}
		free(file->xattrs);
	}
	free(file);
}

void free_dyn(t_dyn *list)
{
	for (int i = 0; i < list->length; i++)
		free_fileData(list->list[i]);
	free(list->list);
}

/* void    free_dyn(t_dyn *list)
{
	for (int i = 0; i < list->length; i++){
		if (list->list[i]->owner){free(list->list[i]->owner);}
		if (list->list[i]->group){free(list->list[i]->group);}
		if (list->list[i]->fileName){free(list->list[i]->fileName);}
		if (list->list[i]->absolutePath){free(list->list[i]->absolutePath);}
		if (list->list[i]->path){free(list->list[i]->path);}
		if (list->list[i]->link_target){free(list->list[i]->link_target);}
		if (list->list[i]->acl_text){free(list->list[i]->acl_text);}
		if (list->list[i]->xattrs) {
			for (int j = 0; j < list->list[i]->xattr_count; j++) {
				if (list->list[i]->xattrs[j].name)
					free(list->list[i]->xattrs[j].name);
			}
			free(list->list[i]->xattrs);
		}
		free(list->list[i]);
		}  
	free(list->list);
} */