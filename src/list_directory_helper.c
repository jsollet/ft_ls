#include "../includes/types.h"
#include "../includes/list_directory_helper.h"

static void initialize_state(t_flags *flags, t_stack **directories_to_process, t_stack **fileList, bool *first_dir)
{
	if (*directories_to_process)
		reverse_stack(directories_to_process);
	if (*fileList)
		reverse_stack(fileList);
	
	

	if (!*first_dir)
		*first_dir = (*directories_to_process && (*directories_to_process)->count > 1);

	if (flags->bigR)
		*first_dir = true;
	if (!*directories_to_process && !*fileList)
		push(directories_to_process, ".");
	if (*directories_to_process && *fileList)
		*first_dir = true;
}

static void handle_argument_files(t_stack **fileList, t_stack **directories_to_process, t_dyn *files, t_context *ctx)
{
	if (fileList && *fileList) {
		process_argument_files(files, directories_to_process, fileList, ctx);
		display_sorted_files(true, files, false, ctx);
		reset_dyn(files);
	}
}

static void handle_flag_d(t_stack **fileList, t_stack **directories_to_process,t_dyn *files, t_context *ctx)
{
	if (fileList)
		process_argument_files(files, NULL, fileList, ctx) ;
	if (directories_to_process)
		process_argument_files(files, NULL, directories_to_process, ctx);
	display_sorted_files(true, files,  false,ctx);
}

static void handle_directories_loop(t_stack **directories_to_process,t_dyn *files, t_context *ctx, bool *first_dir)
{
	bool need_newline = false;
	while (*directories_to_process) {
		char *current_dir = pop(directories_to_process);
		if (need_newline)
			ft_printf("\n");
		process_directory(current_dir, files, directories_to_process, *first_dir, ctx);
		need_newline = true;
		free(current_dir);
		reset_dyn(files);
		*first_dir = true;
	}
}

// utilisee sur getfileinfo je crois
static void update_dynamic_format(t_fileData *file, t_dynamic_format *dyn_format)
{
	size_t len;

	if ((len = ft_strlen(file->fileName)) > dyn_format->max_name_width)
		dyn_format->max_name_width = len;
	if ((len = ft_strlen(file->ownership.owner)) > dyn_format->max_owner_width)
		dyn_format->max_owner_width = len;
	if ((len = ft_strlen(file->ownership.group)) > dyn_format->max_group_width)
		dyn_format->max_group_width = len;
	if ((len = ft_intlen(file->meta.fileSize)) > dyn_format->max_size_width)
		dyn_format->max_size_width = len;
}

static void process_single_file(char *file_path, t_dyn *files, t_stack **directories_to_process, t_context *ctx)
{
	t_fileData *file = malloc_fileData();
	if (!file) {
		perror("malloc");
		set_exit_status(ctx->exit_status, 1, strerror(errno));
		return;
	}

	file->argument = true;
	file->fileName = ft_strdup(file_path);
	file->absolutePath = ft_strdup(file_path);

	get_fileInfo(file_path, file, &files->total_size, ctx/* ->flags, ctx->exit_status, ctx->now, ctx->dyn_format */);
	update_dynamic_format(file, ctx->dyn_format);

	if (file->meta.fileType == 'd' && !ctx->flags->d) {
		push(directories_to_process, file->absolutePath);
	} else {
		append(files, file);
	}
}

static bool validate_directory(const char *path, DIR *dir) {
	struct stat sb;
	if (stat(path, &sb) == -1) {
		perror("stat");
		if (dir) closedir(dir);
		return false;
	}
	if (!S_ISDIR(sb.st_mode)) {
		ft_printf_fd(2, "Not a directory: %s\n", path);
		if (dir) closedir(dir);
		return false;
	}
	return true;
}

static bool process_directory_entry(struct dirent *entry, const char *path, t_dyn *files, t_dyn *subdirs, t_context *ctx)
{
	if (!ctx->flags->a && (entry->d_name[0] == '.' || ft_strcmp(entry->d_name, "..") == 0))
		return true;

	t_fileData *file = create_fileData(path, entry,  &files->total_size, ctx);
	if (!file) {
		if (errno)
			perror("create_fileData");
		else
			ft_printf_fd(2, "Error: create_fileData failed unexpectedly\n");
		return false;
	}

	if (ctx->flags->bigR && file->meta.fileType == 'd' &&
		ft_strcmp(entry->d_name, ".") != 0 &&
		ft_strcmp(entry->d_name, "..") != 0)
	{
		if (!handle_subdir(subdirs, file))
			return false;
	}

	append(files, file);
	return true;
}


void    list_directory(t_flags *flags, t_stack **directories_to_process, t_stack **fileList, t_exit_status *exit_status) {
	time_t now = time(NULL);
	t_dyn files;
	t_dynamic_format dyn_format = {0};
	static bool first_dir = false;

	/* typedef struct s_context {
	t_flags           flags;
	t_exit_status     exit_status;
	time_t            now;
	t_dynamic_format  dyn_format;
	} t_context;	 */
	t_context	context = {.flags = flags, .exit_status = exit_status, .now = now, .dyn_format = &dyn_format};


	init_dyn(&files);
	initialize_state(flags, directories_to_process, fileList, &first_dir);
	handle_argument_files(fileList, directories_to_process, &files,&context);
	if (flags->d){
		handle_flag_d(fileList, directories_to_process, &files, &context);
		free_dyn(&files);
		return;
	}
	handle_directories_loop(directories_to_process, &files, &context, &first_dir);
	free_dyn(&files);
}

void process_argument_files(
	t_dyn *files, t_stack **directories_to_process, t_stack **fileList, t_context *ctx)
{
	while (*fileList != NULL) {
		char *file_path = pop(fileList);
		process_single_file(file_path, files, directories_to_process, ctx);
		free(file_path);
	}
}




void process_directory(const char *dir, t_dyn *files,  t_stack **directories_to_process, bool first_dir, t_context *ctx) {
	bool an_error = list_directory_helper(dir, files, directories_to_process, ctx);
	if (!an_error && (first_dir || ctx->flags->bigR)){
		ft_printf("%s:\n", dir);
	}
	display_sorted_files(an_error, files,  true, ctx);
}



bool list_directory_helper(const char *path, t_dyn *files, t_stack **directories_to_process, t_context *ctx)
{
	DIR *dir = opendir(path);
	if (!validate_directory(path, dir))
		return true;

	if (!dir)
		return handle_dir_open_error(path);

	t_dyn subdirs;
	init_dyn(&subdirs);
	struct dirent *entry;

	while (1) {
		errno = 0;
		entry = readdir(dir);
		if (!entry)
			break;
		if (!process_directory_entry(entry, path, files, &subdirs, ctx)) {
			closedir(dir);
			free_dyn(files);
			free_dyn(&subdirs);
			return false;
		}
	}

	if (errno != 0) {
		ft_printf_fd(2, "ft_ls: %s  ", path);
		perror("");
	}

	closedir(dir);

	if (!ctx->flags->U) {
		int (*cmp)(t_fileData *, t_fileData *) = get_cmp_func(ctx->flags);
		mergeSort_iterative(subdirs.list, subdirs.length, cmp);
	}

	for (int i = subdirs.length - 1; i >= 0; i--) {
		clean_path(subdirs.list[i]->absolutePath);
		push(directories_to_process, subdirs.list[i]->absolutePath);
	}

	free_dyn(&subdirs);
	return false;
}


t_fileData	*create_fileData(const char *dir_path, struct dirent *entry, long *total_size, t_context *ctx){
	t_fileData *file = malloc_fileData();
	if (!file) return NULL;
	file->d_type = entry->d_type;
	file->fileName = ft_strdup(entry->d_name);
	char *fullPath = ft_strjoin_multiple(dir_path, "/", entry->d_name, NULL);
	file->absolutePath = fullPath; 
	errno = 0;
	get_fileInfo(fullPath,file,total_size,ctx/* ->flags, ctx->exit_status, ctx->now, ctx->dyn_format */);
	return file;
}

bool	handle_subdir(t_dyn *subdirs, t_fileData *file) {
	t_fileData *subdir = malloc_fileData();
	if (!subdir) return false;
	subdir->fileName = ft_strdup(file->absolutePath);
	subdir->absolutePath = ft_strdup(file->absolutePath);
	subdir->ownership.owner[0] = '\0';
	subdir->ownership.group[0] = '\0';
	subdir->link_target_buf[0] = '\0';
	subdir->meta.st_mtimes = file->meta.st_mtimes;
	subdir->meta.st_atimes = file->meta.st_atimes;
	subdir->meta.st_mtime_nsec = file->meta.st_mtime_nsec;
	subdir->meta.st_atime_nsec = file->meta.st_atime_nsec;
	subdir->meta.st_ino = file->meta.st_ino;
	append(subdirs, subdir);
	return true;
}

bool handle_dir_open_error(const char *path) {
	ft_printf_fd(1, "%s:\n", path);
	ft_printf("total 0\n");
	ft_printf_fd(2, "ft_ls: %s %s\n", path, strerror(errno));
	return true;
}
