#include "../includes/argument.h"

bool file_exists(const char *path) {
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}

bool	is_directory(const char *path)
{ // a modifier

	struct stat s;
	if (lstat(path, &s) == -1)
	{
		//printf("lstat failed--- %s\n", path);
		if (errno == EACCES)
			printf("Permission denied on: %s\n", path);
		if (errno == ENOTDIR)
		{
		//	printf("'%s' is not a directory\n", path);
			return false; // Ce n'est pas un répertoire
		}
		if (!(errno == ENOTDIR) )
		{
			DIR *dir = opendir(path);
			if (dir == NULL){
				if (errno == ENOENT){
		//			printf("'%s' is not a directory\n", path);
					return false;
				}
				else 
				printf("ft_ls: cannot access '%s': %s\n", path, strerror(errno));
			}
		
		//	printf("'%s' is a directory\n", path);
			return true;
		}
	}
	//printf("---S_ISDIR of %s is %d\n", path,S_ISDIR(s.st_mode) );
	return S_ISDIR(s.st_mode);
}



int find_double_dash(int argc, char *argv[]) {
	int index = 1;
    while (index < argc){
        if (argv[index][0] == '-' && argv[index][1] == '-' && argv[index][2] == '\0') {
			return index; // Position du "--"
		}
        index++;
    }
	return argc; // "--" non trouvé avant -1
}

void process_path(t_stack **dirs, t_stack **files,t_flags *flags, char *path, t_exit_status *exit_status) {
	if (is_directory(path)) {
		push(dirs, path);
	}
	else if (file_exists(path)) {
		push(files, path);
	}
	else {
		struct stat buffer;
		if (stat(path, &buffer) != 0) {
			set_exit_status(exit_status, 2, NULL);
			ft_printf_fd(2,"\nft_ls!: %s: %s\n", path, strerror(errno));
		}
	}
}