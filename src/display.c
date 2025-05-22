
#include "../includes/display.h"
#include "../includes/ft_printf1.h"

static void sort_files_if_needed(t_dyn *files, t_flags *flags){
	if (!flags->U){
		int (*cmp)(t_fileData *, t_fileData *) = get_cmp_func(flags);
		mergeSort_iterative(files->list, files->length, cmp);
	}
}

static void display_file_names(t_dyn *files, t_flags *flags, t_color_rule *color_rules){
	char *reset = RESET_COLOR;
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

static bool display_inacessible_file(int i,t_dyn *files, t_flags *flags, t_dynamic_format *dyn_format){
	if (!files->list[i]->valid){
		if (!flags->g){
			ft_printf1(DISPLAY_FORMAT_INACCESSIBLE, 
				files->list[i]->meta.permission, ' ', "?", 
				(int)dyn_format->max_owner_width,files->list[i]->ownership.owner,
				(int)dyn_format->max_group_width,files->list[i]->ownership.group,
				(int)dyn_format->max_size_width,"?",
				files->list[i]->meta.lastModified,
				files->list[i]->fileName);
		} else {
			ft_printf1(DISPLAY_FORMAT_INACCESSIBLE_G, 
				files->list[i]->meta.permission, ' ', "?", 
				(int)dyn_format->max_group_width,files->list[i]->ownership.group,
				(int)dyn_format->max_size_width,"?",
				files->list[i]->meta.lastModified,
				files->list[i]->fileName);
		}
		ft_printf1("\n");
		return true;
	}
	return false;
}

static void display_directory_total(bool an_error,t_dyn *files, bool is_directory){
	if (is_directory && files->length >= 0  && !an_error) {
			#ifdef __APPLE__
				ft_printf1("total %lld\n", files->total_size);
			#else
				ft_printf1("total %lld\n", files->total_size / 2);
			#endif
		}
}


void display_sorted_files(bool an_error,t_dyn *files, t_flags *flags, bool is_directory, t_dynamic_format *dyn_format) {
	char attribute;
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
		{'-', is_executable, GREEN_COLOR},
		{0, NULL, RESET_COLOR}
	};

	sort_files_if_needed(files, flags);

	if (flags->l) {
		display_directory_total(an_error, files, is_directory);
		
		for (int i = 0; i < files->length; i++) {
			attribute = files->list[i]->xattr.has_xattr;
			if (attribute == ' ') {attribute = files->list[i]->xattr.has_acl;}
			if (attribute == '0') {attribute = ' ';}
			//printf("File: %s xattr: |%c| acl: |%c|\n", files->list[i]->absolutePath, files->list[i]->xattr.has_xattr, files->list[i]->xattr.has_acl);
						
			if (display_inacessible_file(i, files, flags, dyn_format)){continue;}

			if (flags->color){
				color = select_color_new(files->list[i], color_rules);
				if (!flags->g)
					ft_printf1(DISPLAY_FORMAT_LONG_COLOR ,
						files->list[i]->meta.permission, attribute, 
						files->list[i]->meta.linkNumber,  
						(int)dyn_format->max_owner_width,files->list[i]->ownership.owner,
						(int)dyn_format->max_group_width,files->list[i]->ownership.group,
						(int)dyn_format->max_size_width,files->list[i]->meta.fileSize, 
						files->list[i]->meta.lastModified, color,files->list[i]->fileName, reset);
				else
				ft_printf1(DISPLAY_FORMAT_LONG_G_COLOR,
					files->list[i]->meta.permission, attribute, files->list[i]->meta.linkNumber,
					(int)dyn_format->max_group_width,files->list[i]->ownership.group,
					(int)dyn_format->max_size_width,files->list[i]->meta.fileSize,   
					files->list[i]->meta.lastModified,color,files->list[i]->fileName, reset);
				
			} else {
				if (!flags->g){
					ft_printf1(DISPLAY_FORMAT_LONG,
					files->list[i]->meta.permission, attribute, files->list[i]->meta.linkNumber,
					(int)dyn_format->max_owner_width,files->list[i]->ownership.owner, 
					(int)dyn_format->max_group_width,files->list[i]->ownership.group,
					(int)dyn_format->max_size_width,files->list[i]->meta.fileSize, 
				  	files->list[i]->meta.lastModified, files->list[i]->fileName);
				}
				else{			
				ft_printf1(DISPLAY_FORMAT_LONG_G,
					files->list[i]->meta.permission, attribute, files->list[i]->meta.linkNumber,
					(int)dyn_format->max_group_width,files->list[i]->ownership.group,
					(int)dyn_format->max_size_width,files->list[i]->meta.fileSize, 
				  	files->list[i]->meta.lastModified, files->list[i]->fileName);
				}
			}
			
			if (files->list[i]->meta.fileType == 'l' && files->list[i]->link_target_buf[0] != '\0') {
				ft_printf1(" -> %s", files->list[i]->link_target_buf);
			}
			if (flags->e && files->list[i]->xattr.acl_text){
				ft_printf1("\n%s", files->list[i]->xattr.acl_text);
			}
			if (flags->at && files->list[i]->xattr.has_xattr== '@'){
				for (int j=0;j < files->list[i]->xattr.xattr_count; j++){
					ft_printf1("\n\t%s\t%zd\t", files->list[i]->xattr.xattrs[j].name, files->list[i]->xattr.xattrs[j].size);
					print_xattr_value(files->list[i]->xattr.xattrs[j].value,files->list[i]->xattr.xattrs[j].size );
				}
			}
			ft_printf1("\n");
		}
	} else {
		display_file_names(files, flags, color_rules);
	}
}
