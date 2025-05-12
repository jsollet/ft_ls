#include "../includes/types.h"
#include "../includes/list_directory_helper.h"


void    list_directory(t_flags *flags, t_stack **directories_to_process, t_stack **fileList, t_exit_status *exit_status) {
	time_t now = time(NULL);
	t_dyn files;
	init_dyn(&files);
	static t_inodeSet inode_set;
	inode_set_init(&inode_set);
	static bool first_dir = false;
	bool need_newline = false;
	// ajout ici 
	if (*directories_to_process)
		reverse_stack(directories_to_process);
	if (*fileList)
		reverse_stack(fileList);

	if (!first_dir) {  
		first_dir = (*directories_to_process && (*directories_to_process)->count > 1); 
	}
	if (flags->bigR)
		first_dir = true;
	if (( *directories_to_process == NULL && *fileList == NULL )) {
		
		push(directories_to_process, ".");
	}
	if (( *directories_to_process != NULL && *fileList != NULL )){
		first_dir = true;
	}

	if (fileList != NULL) {// avant entre les 2 if 
		process_argument_files(&files, directories_to_process, fileList, flags, exit_status, now);
		display_sorted_files(true,&files, flags, false);
		reset_dyn(&files);
	}	
	if (flags->d){
		if (fileList)
			process_argument_files(&files, NULL, fileList, flags, exit_status, now);
		if (directories_to_process)
			process_argument_files(&files, NULL, directories_to_process, flags, exit_status, now);
		
		display_sorted_files(true, &files, flags, false);
		free_dyn(&files);
		return;
	} else if (!flags->d)
		{
			while(*directories_to_process)
			{
				char *current_dir = pop(directories_to_process);
				if (need_newline)
        			ft_printf("\n");
				process_directory(current_dir, &files, flags,directories_to_process, first_dir, exit_status, &inode_set, now);
				need_newline = true;
				free(current_dir);
				reset_dyn(&files);
				first_dir = true;
			}
		}
	free_dyn(&files);
	inode_set_free(&inode_set);
}


void	process_argument_files(t_dyn *files,t_stack **directories_to_process, t_stack **fileList, t_flags *flag, t_exit_status *exit_status, time_t now){
	while (*fileList != NULL){
		char    *file_path = pop(fileList);
		t_fileData *file = malloc_fileData();
		if (!file) {
			perror("malloc");
			set_exit_status(exit_status, 1, strerror(errno));
			continue;
		}
		
		file->argument = true;
		file->fileName = ft_strdup(file_path);
		file->absolutePath = ft_strdup(file_path);
		get_fileInfo(file_path, file, flag,&files->total_size, exit_status, now ); //
		if (file->fileType == 'd'  && !flag->d) { // ici modif pour -d
			push(directories_to_process, file->absolutePath);
		} else {
			append(files, file);
		}
		free(file_path);
	}
}

void process_directory(const char *dir, t_dyn *files, t_flags *flags, t_stack **directories_to_process, bool first_dir, t_exit_status *exit_status, t_inodeSet *inode_set, time_t now) {
	bool an_error = list_directory_helper(dir, files, flags, directories_to_process, exit_status, inode_set, now);
	if (!an_error && (first_dir || flags->bigR))// ajout 
		ft_printf("%s:\n", dir);
	display_sorted_files(an_error, files, flags, true);
}

bool    list_directory_helper(const char *path, t_dyn *files, t_flags *flags, t_stack **directories_to_process, t_exit_status *exit_status, t_inodeSet *inode_set, time_t now) {
	bool status;
	(void)inode_set;
	DIR *dir = opendir(path);


	struct stat sb;
	if (stat(path, &sb) == -1) {
    perror("stat");
    closedir(dir);
    return true; // erreur
	}
	if (!S_ISDIR(sb.st_mode)) {
    fprintf(stderr, "Not a directory: %s\n", path);
    closedir(dir);
    return true;
	}

	if (!dir)
	{
		return handle_dir_open_error(path); // true si erreur
	}


	t_dyn subdirs;
	init_dyn(&subdirs); 
	
	struct dirent *entry;
	
	while (1){
		errno = 0;
		entry = readdir(dir);
    		if (!entry){
        		break;
		}

		if (!flags->a && (ft_strcmp(entry->d_name, "..") == 0 || entry->d_name[0] == '.')) { continue;}

		t_fileData *file = create_fileData(path, entry, flags, &files->total_size, exit_status, now);
		if (!file){
			if (errno)
        		perror("create_fileData");
				else
			ft_printf_fd(2, "Error: create_fileData failed unexpectedly\n");
			closedir(dir);
			free_dyn(files);
			status = false;//false;// me demande si plutot true ici
			return status;
		}

		/* if (inode_set_contains(inode_set, file->st_ino )){
			free_fileData(file);
			continue;
		} 
		inode_set_add(inode_set, entry->d_ino); */
		// Si c'est un répertoire, l'ajouter à la pile
		if (flags->bigR && file->fileType == 'd' && ft_strcmp(entry->d_name, "..") != 0 && ft_strcmp(entry->d_name, ".") != 0)
		{
			status = handle_subdir(&subdirs, file); // met dans absolutePath
		}
		append(files, file);

		
	}

	if (errno != 0) {
		fprintf(stderr, "readdir failed on path: %s → ", path);
		perror("readdir");
	}

	closedir(dir);

	if (!flags->U){
		int (*cmp)(t_fileData *, t_fileData *) = get_cmp_func(flags);
		mergeSort_iterative(subdirs.list, subdirs.length, cmp); // ici un probleme je pense on change l'ordre de for, et cmp
	}

	for (int i = subdirs.length - 1; i >= 0; i--) {
		clean_path(subdirs.list[i]->absolutePath);
		push(directories_to_process,  subdirs.list[i]->absolutePath);
	}

	free_dyn(&subdirs);
	return false; // pas d'erreur
}


t_fileData	*create_fileData(const char *dir_path, struct dirent *entry,t_flags *flag, long *total_size, t_exit_status *exit_status, time_t now){
	t_fileData *file = malloc_fileData();
	if (!file) return NULL;

	file->d_type = entry->d_type;
	file->fileName = ft_strdup(entry->d_name);
	char *fullPath = ft_strjoin_multiple(dir_path, "/", entry->d_name, NULL);
	//ft_strlcpy(file->absolutePath, fullPath,PATH_MAX + 1);
	file->absolutePath = fullPath;//ft_strdup(fullPath); 

	errno = 0;
	get_fileInfo(fullPath,file,flag,total_size, exit_status, now);
	//free(fullPath);
	return file;
}

bool	handle_subdir(t_dyn *subdirs, t_fileData *file) {

	t_fileData *subdir = malloc_fileData();
	if (!subdir) return false;
	//subdir->fileName = ft_strdup(file->fileName);
	//clean_path(file->absolutePath);

	subdir->fileName = ft_strdup(file->absolutePath); // On stocke juste le chemin absolu, pour le tri !
	//ft_strlcpy(subdir->absolutePath, file->absolutePath, sizeof(file->absolutePath) );//
	subdir->absolutePath = ft_strdup(file->absolutePath);
	subdir->owner[0] = '\0';
	subdir->group[0] = '\0';

	subdir->link_target_buf[0] = '\0';
	subdir->st_mtimes = file->st_mtimes;
	subdir->st_mtimes = file->st_atimes;
	subdir->st_ino = file->st_ino;
	append(subdirs, subdir);
	return true;
}

bool handle_dir_open_error(const char *path) {
	ft_printf_fd(1, "%s:\n", path);
	ft_printf("total 0\n");
	ft_printf_fd(2, "ft_ls: %s %s\n", path, strerror(errno));
	return true;
}
