
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

static char get_display_attribute(t_fileData *file) {
    char attribute = file->xattr.has_xattr;
    if (attribute == ' ') attribute = file->xattr.has_acl;
    if (attribute == '0') attribute = ' ';
    return attribute;
}

static void display_long_file_color(char attribute,t_fileData *files, t_flags *flags, t_dynamic_format *dyn_format, t_color_rule *color_rules){
	char *reset = RESET_COLOR;
	char *color = reset;
	color = select_color_new(files, color_rules);
	if (!flags->g){
		ft_printf1(DISPLAY_FORMAT_LONG_COLOR ,
		files->meta.permission, attribute, 
		files->meta.linkNumber,  
		(int)dyn_format->max_owner_width,files->ownership.owner,
		(int)dyn_format->max_group_width,files->ownership.group,
		(int)dyn_format->max_size_width,files->meta.fileSize, 
		files->meta.lastModified, color,files->fileName, reset);
	} else {
		ft_printf1(DISPLAY_FORMAT_LONG_G_COLOR,
			files->meta.permission, attribute, files->meta.linkNumber,
			(int)dyn_format->max_group_width,files->ownership.group,
			(int)dyn_format->max_size_width,files->meta.fileSize,   
			files->meta.lastModified,color,files->fileName, reset);
	}
}

static void display_long_file_link_color(char attribute,t_fileData *files, t_flags *flags, t_dynamic_format *dyn_format, t_color_rule *color_rules){
	char *reset = RESET_COLOR;
	char *color = reset;
	color = select_color_new(files, color_rules);
	int	dyn_size =ft_strlen(files->fileName);

	if (!flags->g){
		ft_printf1(DISPLAY_FORMAT_LONG_LINK_COLOR,
		files->meta.permission, attribute, 
		files->meta.linkNumber,  
		(int)dyn_format->max_owner_width,files->ownership.owner,
		(int)dyn_format->max_group_width,files->ownership.group,
		(int)dyn_format->max_size_width,files->meta.fileSize, 
		files->meta.lastModified, color,dyn_size, files->fileName, files->link_target_buf, reset);
	} else {
		ft_printf1(DISPLAY_FORMAT_LONG_G_LINK_COLOR,
			files->meta.permission, attribute, files->meta.linkNumber,
			(int)dyn_format->max_group_width,files->ownership.group,
			(int)dyn_format->max_size_width,files->meta.fileSize,   
			files->meta.lastModified,color,dyn_size, files->fileName,files->link_target_buf, reset);
	}
}

static void display_long_file( char attribute,t_fileData *files, t_flags *flags, t_dynamic_format *dyn_format){
	if (!flags->g){
		ft_printf1(DISPLAY_FORMAT_LONG,
			files->meta.permission, attribute, files->meta.linkNumber,
			(int)dyn_format->max_owner_width,files->ownership.owner, 
			(int)dyn_format->max_group_width,files->ownership.group,
			(int)dyn_format->max_size_width,files->meta.fileSize, 
			files->meta.lastModified,
			files->fileName);
	} else{			
		ft_printf1(DISPLAY_FORMAT_LONG_G,
			files->meta.permission, attribute, files->meta.linkNumber,
			(int)dyn_format->max_group_width,files->ownership.group,
			(int)dyn_format->max_size_width,files->meta.fileSize, 
			files->meta.lastModified, files->fileName);
	}
}

static void display_long_file_link( char attribute,t_fileData *files, t_flags *flags, t_dynamic_format *dyn_format){
	int	dyn_size =ft_strlen(files->fileName);

	if (!flags->g){
		ft_printf1(DISPLAY_FORMAT_LONG_LINK,
			files->meta.permission, attribute, files->meta.linkNumber,
			(int)dyn_format->max_owner_width,files->ownership.owner, 
			(int)dyn_format->max_group_width,files->ownership.group,
			(int)dyn_format->max_size_width,files->meta.fileSize, 
			files->meta.lastModified,
			dyn_size,files->fileName, files->link_target_buf);
	} else{			
		ft_printf1(DISPLAY_FORMAT_LONG_G_LINK,
			files->meta.permission, attribute, files->meta.linkNumber,
			(int)dyn_format->max_group_width,files->ownership.group,
			(int)dyn_format->max_size_width,files->meta.fileSize, 
			files->meta.lastModified, 
			dyn_size, files->fileName, files->link_target_buf);
	}
}
/* 
static void display_link(t_fileData *files){
	if (files->meta.fileType == 'l' && files->link_target_buf[0] != '\0') {
		ft_printf1(" -> %s", files->link_target_buf);
	}
} */

static void display_acl(t_fileData *files, t_flags *flags){
	if (flags->e && files->xattr.acl_text){
		ft_printf1("\n%s", files->xattr.acl_text);
	}
}

static void display_xattrs(t_fileData *files,  t_flags *flags) {
	if (flags->at && files->xattr.has_xattr== '@'){
    	for (int j = 0; j < files->xattr.xattr_count; j++) {
       		ft_printf1("\n\t%s\t%zd\t", files->xattr.xattrs[j].name, files->xattr.xattrs[j].size);
        	print_xattr_value(files->xattr.xattrs[j].value, files->xattr.xattrs[j].size);
    	}
	}
}

void display_sorted_files(bool an_error,t_dyn *files, bool is_directory, t_context *ctx) {
	char attribute;
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

	sort_files_if_needed(files, ctx->flags);

	if (ctx->flags->l) {
		display_directory_total(an_error, files, is_directory);
		
		for (int i = 0; i < files->length; i++) {
			attribute = get_display_attribute(files->list[i]);		
			
			if (display_inacessible_file(i, files, ctx->flags, ctx->dyn_format)){continue;}
			
			if (ctx->flags->color){
				if (files->list[i]->meta.fileType == 'l' && files->list[i]->link_target_buf[0] != '\0'){
					display_long_file_link_color(attribute, files->list[i], ctx->flags, ctx->dyn_format, color_rules);
				} else {
					display_long_file_color(attribute, files->list[i], ctx->flags, ctx->dyn_format, color_rules);
				}
				
			} else {
				if (files->list[i]->meta.fileType == 'l' && files->list[i]->link_target_buf[0] != '\0'){
					display_long_file_link(attribute, files->list[i], ctx->flags, ctx->dyn_format);
				}
				else {
					display_long_file(attribute, files->list[i], ctx->flags, ctx->dyn_format);
				}
				
			}

			//display_link(files->list[i]);
			display_acl(files->list[i], ctx->flags);
			display_xattrs(files->list[i], ctx->flags);
			ft_printf1("\n");
		}
	} else {
		display_file_names(files, ctx->flags, color_rules);
	}
}
