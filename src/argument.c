#include "../includes/argument.h"

bool file_exists(const char *path) {
	struct stat buffer;
	return (stat(path, &buffer) == 0);
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

void process_path_old(t_stack **dirs, t_stack **files, char *path, t_exit_status *exit_status) {
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

bool process_path(t_stack **dirs, t_stack **files, char *path, t_exit_status *exit_status) {
	struct stat st;
	if (lstat(path, &st) == -1) {
		ft_printf_fd(2, "ft_ls: %s: %s\n", path, strerror(errno));
		if (errno == ENOENT || errno == ENOTDIR) {
			set_exit_status(exit_status, 2, NULL); // Fichier ou dossier inexistant
		} else {
			set_exit_status(exit_status, 1, NULL); // Autre erreur (ex: permission)
		}
		return false;
	}

	if (S_ISDIR(st.st_mode)) {
		push(dirs, path);
        return true;
	} else {
		push(files, path);
        return true;
	}
}
