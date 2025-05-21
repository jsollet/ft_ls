#ifndef ARGUMENT_H
# define ARGUMENT_H
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "../libft/includes/libft.h"
#include "../includes/ministack.h"
#include "./types.h"
#include "./error.h"

bool	file_exists(const char *path);
bool	is_directory(const char *path);
int		find_double_dash(int argc, char *argv[]);
bool	process_path(t_stack **dirs, t_stack **files, char *path, t_exit_status *exit_status);
#endif