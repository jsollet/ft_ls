#ifndef LIST_DIRECTORY_HELPER_H
# define LIST_DIRECTORY_HELPER_H
#include "./types.h"
#include "../includes/ministack.h"
#include "../includes/error.h"
#include "../includes/dynamic_array_string.h"
#include "../includes/utils.h"
#include "../includes/getter.h"
#include "../includes/hashtable.h"
#include "../includes/sort.h"
#include "../includes/display.h"

void	process_argument_files(t_dyn *files,t_stack **directories_to_process, t_stack **fileList, t_flags *flag, t_exit_status *exit_status, time_t now);
void	process_directory(const char *dir, t_dyn *files, t_flags *flags, t_stack **directories_to_process, bool first_dir, t_exit_status *exit_status, t_inodeSet *inode_set, time_t now);
t_fileData	*create_fileData(const char *dir_path, struct dirent *entry,t_flags *flag, long *total_size, t_exit_status *exit_status, time_t now);
bool	handle_subdir(t_dyn *subdirs, t_fileData *file);
bool 	handle_dir_open_error(const char *path);
bool    list_directory_helper(const char *path, t_dyn *files, t_flags *flags, t_stack **directories_to_process, t_exit_status *exit_status, t_inodeSet *inode_set, time_t now);
void    list_directory(t_flags *flags, t_stack **directories_to_process, t_stack **fileList, t_exit_status *exit_status);
#endif