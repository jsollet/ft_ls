
#include "../includes/display.h"

void display_sorted_files(bool an_error,t_dyn *files, t_flags *flags, bool is_directory) {
	char *reset = RESET_COLOR;
	char *color = reset;
	t_color_rule color_rules[] = {
		{'d', is_sticky, BACK_CYAN_COLOR},
		{'-', is_setuid, BACK_RED_COLOR},
    	{'-', is_setgid, BACK_MAGENTA_COLOR},
		{'d', NULL, BLUE_COLOR},
		{'l', NULL, CYAN_COLOR},
		{'s', NULL, MAGENTA_COLOR},
		{'p', NULL, YELLOW_COLOR},
		{'b', NULL, RED_COLOR},
		{'c', NULL, RED_COLOR},
		{'-', is_executable, GREEN_COLOR}, // si fichier normal mais exécutable
		{0, NULL, RESET_COLOR} // Fin de la table
	};

	char extra ;
	char *format = ft_strdup("%-10s %3ld %-10s %10s %7ld %-20s %-30s");
	if (!flags->U){ // trie la liste des fichiers et dossier dans le repertoire courant
		
		int (*cmp)(t_fileData *, t_fileData *) = get_cmp_func(flags);
		mergeSort_iterative(files->list, files->length, cmp);
	}
	//bool first = true;

	if (flags->l) {
		
		if (is_directory && files->length >= 0  && !an_error) {
			#ifdef __APPLE__
				ft_printf("total %lld\n", files->total_size);
			#else
				ft_printf("total %lld\n", files->total_size/ 2);
			#endif
			}
		
		for (int i = 0; i < files->length; i++) {
			if (!files->list[i]->valid){
				//printf(RED_COLOR);
				printf(FLAG_INACCESSIBLE, files->list[i]->permission, extra, "?", files->list[i]->owner,
					files->list[i]->group,"?",files->list[i]->lastModified,files->list[i]->fileName);
				continue;
			} 
			extra = files->list[i]->has_xattr;
			if (extra == ' ')
				extra = files->list[i]->has_acl;
			if (extra == '0')
				extra = ' ' ;
			if (flags->color){
				color = select_color_new(files->list[i], color_rules);
				if (!flags->g)
					printf(FLAG_LCOLOR,files->list[i]->permission, extra, files->list[i]->linkNumber,  files->list[i]->owner,
						files->list[i]->group, files->list[i]->fileSize, files->list[i]->lastModified, color,files->list[i]->fileName, reset);
				else
				printf(FLAG_LGCOLOR,files->list[i]->permission, extra, files->list[i]->linkNumber,  
					files->list[i]->group, files->list[i]->fileSize, files->list[i]->lastModified,color,files->list[i]->fileName, reset);
				
			} else {
				if (!flags->g)
				printf(FLAG_L,files->list[i]->permission, extra, files->list[i]->linkNumber,  files->list[i]->owner,
					files->list[i]->group, files->list[i]->fileSize, files->list[i]->lastModified, files->list[i]->fileName);
				else
				printf(FLAG_LG,files->list[i]->permission, extra, files->list[i]->linkNumber,  
					files->list[i]->group, files->list[i]->fileSize, files->list[i]->lastModified,files->list[i]->fileName);
			}
			
			if (files->list[i]->fileType == 'l' && files->list[i]->link_target_buf[0] != '\0') {
				printf(" -> %s", files->list[i]->link_target_buf);
			}
			if (flags->e && files->list[i]->acl_text){ // ici peut etre pour afficher avec l'option @
				printf("\n%s\n", files->list[i]->acl_text);
			}
			if (flags->at && files->list[i]->has_xattr== '@'){
				for (int j=0;j < files->list[i]->xattr_count; j++){
					printf("\n\t%s\t%zd", files->list[i]->xattrs[j].name, files->list[i]->xattrs[j].size);
				}
			}
			printf("\n");
		}
	} else {
		char *arr[files->length];
		char *color[files->length];
		
		for (int i = 0; i < files->length; i++) {
			arr[i] = files->list[i]->fileName;
			if (flags->color)
				color[i] = select_color_new(files->list[i], color_rules);
			else
				color[i] = reset;
		}
		if (!flags->one)
			list_directory_column(arr, color,  files->length);
		else
			list_directory_line(arr, color, files->length);
	}
	free(format);
}