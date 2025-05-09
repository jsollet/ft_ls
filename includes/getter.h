#ifndef GETTER_H
# define GETTER_H
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <stddef.h>
#include "../libft/includes/libft.h"
#include "../includes/ministack.h"
#include "../includes/error.h"
#include "../includes/argument.h"
#include "../includes/bonus.h"
#include "./types.h"
#include "./error.h"



void	get_owner_group(struct stat *sfile, char *owner, size_t owner_size, char *group, size_t group_size, t_exit_status *exit_status);
char    get_file_type(mode_t mode);
void    get_permissions(mode_t mode, char *permission);
void	apply_special_bits(mode_t mode, char *perm);
void	get_symlink_target(const char *path,char *link_target_buf, size_t buf_size , t_exit_status *exit_status);

bool	fill_stat_data(const char *path, struct stat *sfile, t_fileData *file,  t_exit_status *exit_status);
void	fill_basic_info(t_fileData *file, struct stat *sfile, long *total_size);
void	fill_user_group_info(t_fileData *file, struct stat *sfile, t_exit_status *exit_status);
void	fill_inaccessible_fileInfo(t_fileData *file, const char *name);
void	fill_permissions(t_fileData *file, struct stat *sfile);
void	fill_extended_attrs(t_fileData *file, t_flags *flag, t_exit_status *exit_status);
void	fill_last_modified(t_fileData *file, const struct stat *sfile, char flag_label, time_t now);
void	fill_symlink_target(const char *path, t_fileData *file, t_exit_status *exit_status);

void    get_fileInfo(const char* path, t_fileData *file,  t_flags *flag,long *total_size, t_exit_status *exit_status, time_t now);
#endif