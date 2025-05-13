#include "../includes/argument.h"

bool file_exists(const char *path) {
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}

bool is_directory(const char *path) {
    struct stat s;

    if (lstat(path, &s) == -1) {
        return false;
    }

    return S_ISDIR(s.st_mode);
}

int find_double_dash(int argc, char *argv[]) {
	int index = 1;
    while (index < argc){
        if (argv[index][0] == '-' && argv[index][1] == '-' && argv[index][2] == '\0') {
			return index;
		}
        index++;
    }
	return argc;
}



bool process_path(t_stack **dirs, t_stack **files, char *path, t_exit_status *exit_status) {
	struct stat st;
	if (lstat(path, &st) == -1) {
		ft_printf_fd(2, "ft_ls: %s: %s\n", path, strerror(errno));
		if (errno == ENOENT || errno == ENOTDIR) {
			set_exit_status(exit_status, 2, NULL);
		} else {
			set_exit_status(exit_status, 1, NULL);
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
