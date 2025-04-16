#include "../includes/argument.h"

bool file_exists(const char *path) {
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}

bool	is_directory_slow(const char *path)
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

bool is_directory(const char *path) {
    struct stat s;
    
    // On tente d'abord d'obtenir les infos avec lstat (qui est plus rapide)
    if (lstat(path, &s) == -1) {
        if (errno == EACCES) {
            printf("Permission denied on: %s\n", path);
			//ft_printf("\n-----LSTAT|%s|\n", path);
        }
		//ft_printf("\n-----LSTAT|%s|\n", path);
        return false;  // En cas d'erreur, considérons que ce n'est pas un répertoire
    }

    // Si c'est un répertoire selon les infos de stat, on retourne vrai directement
    if (S_ISDIR(s.st_mode)) {
        return true;
    }

    // Sinon, on effectue un dernier contrôle avec opendir
    DIR *dir = opendir(path);
    if (dir != NULL) {
        closedir(dir);  // On ferme immédiatement le répertoire ouvert
        return true;
    }
	//ft_printf("\n-----OPENDIR|%s|\n", path);
    return false;  // Ce n'est ni un répertoire, ni accessible
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

void process_path(t_stack **dirs, t_stack **files, char *path, t_exit_status *exit_status) {
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