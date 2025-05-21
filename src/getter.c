#include "../includes/getter.h"

t_special_bit specials[] = {
	{S_ISUID, S_IXUSR, 's', 'S', 3},
	{S_ISGID, S_IXGRP, 's', 'S', 6},
	{S_ISVTX, S_IXOTH, 't', 'T', 9}
};

void get_owner_group(struct stat *sfile, char *owner, size_t owner_size,
	char *group, size_t group_size, t_exit_status *exit_status)
{
	bool error = false;
	struct passwd *pwd = getpwuid(sfile->st_uid);
	struct group  *grp = getgrgid(sfile->st_gid);
	if (pwd)
		ft_strlcpy(owner, pwd->pw_name, owner_size);
	else {
		ft_strlcpy(owner, "UNKNOWN", owner_size);
		error = true;
	}
	if (grp)
		ft_strlcpy(group, grp->gr_name, group_size);
	else {
		ft_strlcpy(group, "UNKNOWN", group_size);
		error = true;
	}
	if (error)
		set_exit_status(exit_status, 1, NULL);
}

char    get_file_type(mode_t mode){
	if (S_ISDIR(mode)) return 'd';
	if (S_ISLNK(mode)) return 'l';
	if (S_ISREG(mode)) return '-';
	if (S_ISCHR(mode)) return 'c';
	if (S_ISBLK(mode)) return 'b';
	if (S_ISFIFO(mode)) return 'p';
	if (S_ISSOCK(mode)) return 's';
	return '?';
}

void    get_permissions(mode_t mode, char *permission) {
	if (mode & S_IRUSR) permission[1] = 'r';
		else permission[1] = '-';
	if (mode & S_IWUSR) permission[2] = 'w';
		else permission[2] = '-';
	if (mode & S_IXUSR) permission[3] = 'x';
		else permission[3] = '-';
	if (mode & S_IRGRP) permission[4] = 'r';
		else permission[4] = '-';
	if (mode & S_IWGRP) permission[5] = 'w';
		else permission[5] = '-';
	if (mode & S_IXGRP) permission[6] = 'x';
		else permission[6] = '-';
	if (mode & S_IROTH) permission[7] = 'r';
		else permission[7] = '-';
	if (mode & S_IWOTH) permission[8] = 'w';
		else permission[8] = '-';
	if (mode & S_IXOTH) permission[9] = 'x';
		else permission[9] = '-';
	permission[10] = '\0';
	apply_special_bits(mode, permission);
}

void apply_special_bits(mode_t mode, char *perm) {
	for (int i = 0; i < 3; ++i) {
		t_special_bit sb = specials[i];
		if (mode & sb.bit)
			perm[sb.pos] = (mode & sb.exec) ? sb.set : sb.no_exec;
		else
			perm[sb.pos] = (mode & sb.exec) ? 'x' : '-';
	}
}

void	get_symlink_target(const char *path,char *link_target_buf, size_t buf_size , t_exit_status *exit_status){
	ssize_t len = readlink(path, link_target_buf, buf_size - 1);
	if (len != -1){
		link_target_buf[len] = '\0';
	} else {
		link_target_buf[0] = '\0';
		set_exit_status(exit_status, 1, NULL);       
	}
}


bool	fill_stat_data(const char *path, struct stat *sfile, t_fileData *file,  t_exit_status *exit_status){
	if (lstat(path, sfile) == -1) {
		fill_inaccessible_fileInfo(file, path);
		ft_printf_fd(2, "ft_ls: %s: %s\n", path, strerror(errno)); 
		set_exit_status(exit_status, 1, strerror(errno));
		return false;
	}
	return true;
}


void	fill_basic_info(t_fileData *file, struct stat *sfile, long *total_size){
	file->meta.fileSize = sfile->st_size;
	file->meta.linkNumber = sfile->st_nlink;
	file->meta.blocSize = sfile->st_blocks;
	file->meta.st_mtimes = sfile->st_mtime;
	file->meta.st_atimes = sfile->st_atime;
	#ifdef __APPLE__
		file->meta.st_mtime_nsec = sfile->st_mtimespec.tv_nsec;
	#else
		file->meta.st_mtime_nsec = sfile->st_mtim.tv_nsec;
	#endif
	file->meta.st_ino = sfile->st_ino;
	file->xattr.xattrs = NULL;
	file->xattr.xattr_count = 0;
	file->xattr.has_xattr = '0';
	*total_size += file->meta.blocSize;
}

void	fill_user_group_info(t_fileData *file, struct stat *sfile, t_exit_status *exit_status){
	get_owner_group(sfile, file->ownership.owner, sizeof(file->ownership.owner),file->ownership.group, sizeof(file->ownership.group), exit_status);	
}

char convert_d_type_to_char(unsigned char d_type) {
	switch (d_type) {
		case DT_DIR:  return 'd';
		case DT_REG:  return '-';
		case DT_LNK:  return 'l';
		case DT_SOCK: return 's';
		case DT_FIFO: return 'p';
		case DT_CHR:  return 'c';
		case DT_BLK:  return 'b';
		default:      return '?';
	}
}

void	fill_inaccessible_fileInfo(t_fileData *file, const char *name){
	ft_memset(file->meta.permission, '?', 10); file->meta.permission[10] = '\0';
	ft_memset(file->meta.lastModified, '?',19); file->meta.lastModified[19] = '\0';
	if (file->d_type != DT_UNKNOWN) {
        file->meta.permission[0] = convert_d_type_to_char(file->d_type);
    } else {       
        if (is_directory(name)) {
            file->meta.permission[0] = 'd';
        } else {
            file->meta.permission[0] = '-';
        }
    }
	file->meta.fileSize = 0;
	file->meta.linkNumber = 0;
	file->ownership.owner[0] = '?';file->ownership.owner[1] = '\0';
	file->ownership.group[0] = '?';file->ownership.group[1] = '\0';
	file->meta.lastModified[0] = '?';file->meta.lastModified[1] = '\0';
	file->valid = false;
}

void fill_permissions(t_fileData *file, struct stat *sfile){
	file->meta.fileType =get_file_type(sfile->st_mode);
	file->meta.permission[0] = file->meta.fileType;
	get_permissions(sfile->st_mode, file->meta.permission);
}

void fill_extended_attrs(t_fileData *file, t_flags *flag, t_exit_status *exit_status){
	if (ft_strcmp(file->fileName, ".") == 0 || ft_strcmp(file->fileName, "..") == 0) {
		return;
	}
	file->xattr.acl_text = NULL;
	file->xattr.has_xattr = ' ';
	file->xattr.has_acl = ' ';
	if (flag->attr || flag->extended ||  flag->at) {
		file->xattr.has_xattr= has_xattr(file->absolutePath, exit_status);
		if (file->xattr.has_xattr == '@') {
			get_xattr(file, exit_status);
		}
	}
	char *tmp = NULL;
	if (flag->acl || flag->extended || flag->e) {
		file->xattr.has_acl = has_acl(file->absolutePath, &tmp, exit_status);
		if (file->xattr.has_acl == '?') {file->xattr.has_acl = ' ';}//
		if (file->xattr.has_acl == '+')
		{
			#ifdef __APPLE__
				file->xattr.acl_text = format_acl_text(tmp);
			#else
				file->xattr.acl_text = format_acl_text_linux(tmp);
			#endif
		}
		acl_free(tmp);
	}
}

void fill_last_modified(t_fileData *file, const struct stat *sfile, char flag_label, time_t now) {
	time_t timeStamp = (flag_label == 'u') ? sfile->st_atime : sfile->st_mtime;
	const char *timeStr = ctime(&timeStamp);
	if (!timeStr) {
		file->meta.lastModified[0] = '\0';
		return;
	}
	char timeBuf[26];
	ft_memcpy(timeBuf, timeStr, 25);
	timeBuf[24] = '\0';
	if (now - timeStamp > SIX_MONTHS_IN_SECONDS) {
		ft_strlcpy(file->meta.lastModified, timeBuf + 4, 8);
		ft_strlcat(file->meta.lastModified, " ", sizeof(file->meta.lastModified));
		ft_strlcat(file->meta.lastModified, timeBuf + 20, sizeof(file->meta.lastModified));
	} else {
		ft_strlcpy(file->meta.lastModified, timeBuf + 4, 13);
	}
}


void fill_symlink_target(const char *path, t_fileData *file, t_exit_status *exit_status) {
	if (file->meta.fileType == 'l'){
		get_symlink_target(path, file->link_target_buf, sizeof(file->link_target_buf), exit_status);		
	}
	else {
		file->link_target_buf[0] = '\0';
	}		
}

void    get_fileInfo(const char* path, t_fileData *file,  t_flags *flag,long *total_size, t_exit_status *exit_status, time_t now,  t_dynamic_format *dyn_format){
	
	struct stat sfile;
	size_t result;
	char	flag_label;
	if (flag->u) flag_label = 'u';
	else flag_label = 't';

	if (!fill_stat_data(path, &sfile, file, exit_status))
		return;
	fill_basic_info(file, &sfile, total_size);
	fill_user_group_info(file, &sfile, exit_status);
	fill_permissions(file, &sfile);
	if (flag->acl || flag->attr || flag->extended || flag->e || flag->at){
		fill_extended_attrs(file, flag, exit_status);
	}
	fill_last_modified(file, &sfile, flag_label, now);
	fill_symlink_target(path, file, exit_status);
	if ((result = ft_strlen(file->fileName)) > dyn_format->max_name_width) {
		dyn_format->max_name_width = result;
	}
	if ((result = ft_strlen(file->ownership.owner)) > dyn_format->max_owner_width) {
		dyn_format->max_owner_width = result;
	}
	if ((result = ft_strlen(file->ownership.group)) > dyn_format->max_group_width) {
		dyn_format->max_group_width = result;
	}
	if ((result = ft_intlen(file->meta.fileSize)) > dyn_format->max_size_width) {
		dyn_format->max_size_width = result;
	}
} 