#include "../libft/includes/libft.h"
#include "../includes/types.h"
#include "../includes/sort.h"
#include "../includes/dynamic_array_string.h"
#include "../includes/bonus.h"
#include "../includes/ministack.h"
#include "../includes/error.h"
#include "../includes/argument.h"

#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>


bool    list_directory_helper(const char *path, t_dyn *files, t_flags *flags, t_stack **stack, t_exit_status *exit_status);
void	fill_fake_fileInfo(t_fileData *file, const char *name);
t_special_bit specials[] = {
	{S_ISUID, S_IXUSR, 's', 'S', 3},
	{S_ISGID, S_IXGRP, 's', 'S', 6},
	{S_ISVTX, S_IXOTH, 't', 'T', 9}
};

t_fileData	*malloc_fileData(void)
{
	t_fileData *file;
	file = malloc(sizeof(t_fileData));
	if (!file)
		return NULL;
	ft_bzero(file, sizeof(t_fileData));
	file->valid = false; // a voir
	return (file);
}

bool is_directory_test(const char *path)
{
	struct stat s;
	
	// Tenter d'obtenir des informations sur le fichier
	if (lstat(path, &s) == -1)
	{
		// Si lstat échoue, on affiche un message d'erreur
		perror("lstat failed");
		
		if (errno == EACCES)
		{
			printf("Permission denied on: %s\n", path);
		}

		// Si l'erreur est "not a directory", ce n'est pas un répertoire
		if (errno == ENOTDIR)
		{
			printf("'%s' is not a directory\n", path);
			return false; // Ce n'est pas un répertoire
		}

		// Si l'erreur n'est pas ENOTDIR, il pourrait s'agir d'un répertoire mais inaccessible
		DIR *dir = opendir(path);
		if (dir == NULL)
		{
			printf("Failed to open directory: %s\n", path);
			printf("'%s' cannot be accessed: %s\n", path, strerror(errno));
			return false; // Impossible d'ouvrir le répertoire
		}
		else
		{
			printf("'%s' is a directory (but we couldn't access with lstat)\n", path);
			closedir(dir);
			return true; // C'est un répertoire
		}
	}

	// Si lstat réussit, on vérifie si c'est un répertoire
	if (S_ISDIR(s.st_mode))
	{
		printf("'%s' is a directory\n", path);
		return true;  // C'est un répertoire
	}
	
	printf("'%s' is not a directory\n", path);
	return false;  // Ce n'est pas un répertoire
}
/* 
bool	is_directory(const char *path)
{ // a modifier

	struct stat s;
	if (lstat(path, &s) == -1)
	{
		printf("lstat failed--- %s\n", path);
		if (errno == EACCES)
			printf("Permission denied on: %s\n", path);
		if (errno == ENOTDIR)
		{
			printf("'%s' is not a directory\n", path);
			return false; // Ce n'est pas un répertoire
		}
		if (!(errno == ENOTDIR) )
		{
			DIR *dir = opendir(path);
			if (dir == NULL){
				if (errno == ENOENT){
					printf("'%s' is not a directory\n", path);
					return false;
				}
				else 
				printf("ft_ls: cannot access '%s': %s\n", path, strerror(errno));
			}
		
			printf("'%s' is a directory\n", path);
			return true;
		}
	}
	//printf("---S_ISDIR of %s is %d\n", path,S_ISDIR(s.st_mode) );
	return S_ISDIR(s.st_mode);
}
 */
bool    is_directory_old(const char *path){
	DIR *dir = opendir(path);
	if (dir == NULL) {return false;}
	closedir(dir);    
	return true;
}

char    *getPath(const char *file_name){// risque de leak
	size_t  len = ft_strlen(file_name);
	while (len>0) {
		if (file_name[len-1] == '/') break;
		len--; 
	}
	if (len == 0) {
		return ft_strdup("./");
	}
	return ft_substr(file_name, 0, len);
}

char    *getFileName(const char *file_name){// risque de leak
	if (!file_name) return NULL; // Sécurité
	size_t  length = ft_strlen(file_name);
	if (length == 0) return NULL; // Cas vide

	// Si le chemin se termine par '/', on l'ignore a voir
	while (length > 0 && file_name[length - 1] == '/') {
		length--;
	}
	size_t  len = length;
	while (len>0) {
		if (file_name[len-1] == '/') break;
		len--; 
	}
	if (len == 0) {
		return ft_strdup(file_name);
	}
	return ft_substr(file_name, len, length - len);
}

// gpt pour enlever les "//" mais a voir si utile ?
void clean_path(char *path) {
	char *src = path, *dst = path;
	int was_slash = 0;

	while (*src) {
		if (*src == '/') {
			if (!was_slash) {
				*dst++ = *src;
				was_slash = 1;
			}
		} else {
			*dst++ = *src;
			was_slash = 0;
		}
		src++;
	}
	*dst = '\0';
}


// me semble foireux

// GPT pour imprimer correctement ?? 
void print_relative_path(int fd,const char *full_path, const char *start_path) {
	if (ft_strncmp(full_path, start_path, ft_strlen(start_path)) == 0) {
		full_path += ft_strlen(start_path); // Ignore le préfixe start_path
		while (*full_path == '/') full_path++; // Évite les `/` superflus
	}
	ft_printf_fd(fd,"./%s:", full_path);
}



void check_files_exist(int argc, char *argv[]){
	struct stat buffer;
	int index = 1;
	while (index < argc){
		if (argv[index][0] == '-'){
			index++;
			continue;
		}
		if (stat(argv[index], &buffer) != 0) {
			ft_printf_fd(2,"\nft_ls: %s: %s\n", argv[index], strerror(errno));
		}
		index++;
	}
}

void    print_flag(t_flags *flags){
	ft_printf("R[%d] r[%d] a[%d] l[%d] t[%d] u[%d] f[%d] g[%d] d[%d] e[%d] @[%d] u[%d] U[%d] f[%d]", 
		flags->bigR,
		flags->r,
		flags->a,
		flags->l,
		flags->t,
		flags->u,
		flags->f,
		flags->g,
		flags->d,
		flags->e,
		flags->at,
		flags->u,
		flags->U,
		flags->f
	);
}

bool    parse_flags(int argc, char *argv[], t_flags *flags){
	int index = 1;
	while (index < argc){
		if (argv[index][0] == '-' && argv[index][1] != '\0'){
			int position = 1;
			while (argv[index][position] != '\0'){
				if (argv[index][position] == 'R') flags->bigR = true;
				else if (argv[index][position] == 'r') flags->r = true;
				else if (argv[index][position] == 'a') flags->a = true;
				else if (argv[index][position] == 'l') flags->l = true;
				else if (argv[index][position] == 't') flags->t = true;
				else if (argv[index][position] == 'u') flags->u = true;
				else if (argv[index][position] == 'g') {flags->g = true; flags->l = true;}
				else if (argv[index][position] == 'd') flags->d = true;
				else if (argv[index][position] == 'u') flags->u = true;  //bonus acl
				else if (argv[index][position] == 'e') flags->e = true;  //bonus acl
				else if (argv[index][position] == '@') flags->at = true;  //bonus acl
				else if (argv[index][position] == 'U') flags->U = true;
				else if (argv[index][position] == 'f') {flags->f = true; flags->U = true; flags->a = true;} 
				else {
					errno = EINVAL;
					ft_printf_fd(2,"ls: invalid option -- '%c'\n", argv[index][position]);
					ft_printf_fd(2,"ft_ls error: %s\n", strerror(errno));
					ft_printf_fd(2, "usage: ls [-@RUalrtufgde] [file ...]\n");
					return false; //voir pour les erreur et leurs gestion
				}
				position++;
			}
		}
		else if (argv[index][0] == '-' && argv[index][1] == '\0'){
			errno = EINVAL;
			ft_printf_fd(2, "ls: invalid option -- '-'\n");
			ft_printf_fd(2,"ft_ls error: %s\n", strerror(errno));
			ft_printf_fd(2, "usage: ls [-@RUalrtufgde] [file ...]\n");
			return false;
		}
		index++;
	}
	return true;
}



// A voir ces 3 là et les comprendre puis les inserer
void    init_terminal(t_term *t){
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &t->w);
	t->term_width = t->w.ws_col;
	t->col_width = 0;  // À définir plus tard
	t->count = 0;
	t->max_len = 0;
}

void    update_terminal(t_term *t, char **files, size_t file_count) {
	t->count = file_count;
	for (size_t i = 0; i < file_count; i++) {
		size_t len = ft_strlen(files[i]);
		if (len > t->max_len)
			t->max_len = len;
	}
	t->col_width = t->max_len + 5;  // Ajout d'un espace pour la lisibilité ici avant 2, maintenant ok 
}

void    print_files_in_columns(char **files, t_term *t) {
	int cols = t->term_width / t->col_width;
	if (cols == 0) cols = 1;  // Sécurité pour éviter une division par zéro

	int rows = (t->count + cols - 1) / cols;
	
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			size_t index = col * rows + row;
			if (index < t->count) {  // S'assurer que l'indice est valide
				// Afficher le fichier à la position donnée
				write(1, files[index], ft_strlen(files[index]));

				// Ajouter un espace après chaque fichier pour les séparer
				int space_count = t->col_width - ft_strlen(files[index]);
				for (int j = 0; j < space_count; j++) {
					write(1, " ", 1);
				}
			}
		}
		// Passer à la ligne après avoir imprimé une ligne entière
		write(1, "\n", 1);
	}
}


// a continuer
// ERREUR,MODIFICATION del la structure tableau dynamique
void    list_directory_column( char *arr[], int arr_length){
	t_term t;
	init_terminal(&t);
	update_terminal(&t, arr, arr_length);
	print_files_in_columns(arr, &t);
}



/* long long    get_bloc_size(const char *path, const char *fileName){
	struct stat fileStat;

	char *temp = ft_strjoin(path,"/");
	char *temp1 = ft_strjoin(temp, fileName); 
	free(temp);

	if (lstat(temp1, &fileStat) == -1){
		perror("lstat");
	}
	free(temp1);
	return fileStat.st_blocks;
} */

void	get_owner_group(struct stat *sfile, char **owner, char **group,  t_exit_status *exit_status){

	struct passwd *pwd = getpwuid(sfile->st_uid);
	struct group *grp = getgrgid(sfile->st_gid);

	if (pwd) *owner = ft_strdup( pwd->pw_name);
		else {
			*owner = ft_strdup("UNKNOWN");
			set_exit_status(exit_status, 1, NULL);
		}
		
	if (grp) *group = ft_strdup(grp->gr_name);
		else {
			*group = ft_strdup("UNKNOWN");
			set_exit_status(exit_status, 1, NULL);
		}
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

void apply_special_bits(mode_t mode, char *perm) {
	for (int i = 0; i < 3; ++i) {
		t_special_bit sb = specials[i];
		if (mode & sb.bit)
			perm[sb.pos] = (mode & sb.exec) ? sb.set : sb.no_exec;
		else
			perm[sb.pos] = (mode & sb.exec) ? 'x' : '-';
	}
}

void    get_permissions(mode_t mode, char *permission){
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

void	get_symlink_target(const char *path, char **link_target , t_exit_status *exit_status){
	char buffer[PATH_MAX];
	ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);

	if (len != -1){
		buffer[len] = '\0';
		*link_target = ft_strdup(buffer);
		if (*link_target == NULL)
			set_exit_status(exit_status, 1, NULL);
	} else {
		*link_target = NULL;
		set_exit_status(exit_status, 1, NULL);       
	}
}


// fonction pour get_fileInfo
bool	fill_stat_data(const char *path, struct stat *sfile, t_fileData *file,  t_exit_status *exit_status){ // ajout de t_fileData *file
	//ft_printf("\n--|%s|\n", path);
	if (lstat(path, sfile) == -1) {
		ft_printf("\n--|%s|\n", path);
		fill_fake_fileInfo(file, path);
		set_exit_status(exit_status, 1, strerror(errno));
		
		return false;
	}
	return true;
}

void	fill_basic_info(t_fileData *file, struct stat *sfile, long *total_size){
	file->fileSize = sfile->st_size;
	file->linkNumber = sfile->st_nlink;
	file->blocSize = sfile->st_blocks;
	file->st_mtimes = sfile->st_mtime;
	file->st_atimes = sfile->st_atime;
	file->st_ino = sfile->st_ino;
	// foireux VV
	file->xattrs = NULL;
	file->xattr_count = 0;
	file->has_xattr = '0';
	// foireux ^^
	*total_size += file->blocSize;
}

void	fill_user_group_info(t_fileData *file, struct stat *sfile, t_exit_status *exit_status){
	get_owner_group(sfile, &file->owner, &file->group, exit_status);	
}

void fill_permissions(t_fileData *file, struct stat *sfile){
	file->fileType =get_file_type(sfile->st_mode);
	file->permission[0] = file->fileType;
	get_permissions(sfile->st_mode, file->permission);
}

void fill_extended_attrs(t_fileData *file, t_exit_status *exit_status){
	
	char *tmp = NULL;
	file->acl_text = NULL;
	
	file->has_xattr= has_xattr(file->absolutePath, exit_status);
	//printf("%s has attribute [%c]\n",file->absolutePath,file->has_xattr );

	if (file->has_xattr == '?') {
		file->has_xattr = ' ';
		//return 1;
	} //
	if (file->has_xattr == '@') {
		get_xattr(file, exit_status);
		//if (status == 1){return 1;}
	}
	if (file->has_xattr == ' '){
		file->has_acl = has_acl(file->absolutePath, &tmp, exit_status);
		if (file->has_acl == '?') {file->has_acl = ' ';}//
		if (file->has_acl == '+')
		{
			file->acl_text = format_acl_text(tmp);
			free(tmp);
		}
	}
	//return 0;
}

// modification pour le falg u
void fill_last_modified(t_fileData *file, struct stat *sfile, char flag_label){
	time_t	now = time(NULL);
	time_t	timeStamp;
	if (flag_label == 'u')
		timeStamp = sfile->st_atime;
	else 
		timeStamp = sfile->st_mtime;

	char    *timeStr = ctime(&timeStamp);// sfile->st_mtime
	if (timeStr){
		timeStr[ft_strlen(timeStr) - 1] = '\0'; 
		if (now - timeStamp > 6 * 30 * 24 * 60 * 60) {// 15552000 je crois
			// Si la date est plus vieille que 6 mois, on formate comme "Mmm dd yyyy"
			ft_strlcpy(file->lastModified, timeStr + 4, 8);  // Mois et jour
			ft_strlcat(file->lastModified, " ", 20);
			ft_strlcat(file->lastModified, timeStr + 20, 20); // Année
		} else {
			// Si la date est récente, on formate comme "Mmm dd HH:MM"
			ft_strlcpy(file->lastModified, timeStr + 4, 13);  // Mois, jour, heure et minute 13 a la place de 12
		}
	} else {
		file->lastModified[0] = '\0';
	}
}

void fill_symlink_target(const char *path, t_fileData *file, t_exit_status *exit_status) {
	if (file->fileType == 'l'){
		get_symlink_target(path, &file->link_target, exit_status);
	}
	else {
		file->link_target = NULL;
	}		
}

void	fill_fake_fileInfo(t_fileData *file, const char *name){ // a modifier
	(void)name;
	//file->fileName = ft_strdup(getFileName(name)); //
	//file->absolutePath = ft_strdup(name);
	printf("---------------%s\n", name);
	ft_memset(file->permission, '?', 10); file->permission[10] = '\0';
	ft_memset(file->lastModified, '?',19); file->lastModified[19] = '\0';
	
	if (is_directory(name)){
		file->permission[0] = 'd';
	} else {
		file->permission[0] = '-';
	}

	file->fileSize = 0;
	file->linkNumber = 0;
	file->owner = ft_strdup("?");
	file->group = ft_strdup("?");
	file->valid = false; // a voir
}

void    get_fileInfo(const char* path, t_fileData *file,  t_flags *flag,long *total_size, t_exit_status *exit_status){
	
	struct stat sfile;

	char	flag_label;
	if (flag->u) flag_label = 'u';
	else flag_label = 't';

	if (!fill_stat_data(path, &sfile, file, exit_status))
		return;
		//printf("!!|%s|  %s\n",path,  exit_status->message);
	if (exit_status->code == 1 && exit_status->message && ft_strncmp("FAKE", exit_status->message, 4) == 0){
		//printf("\n----------FAKE %s\n", exit_status->message);
		fill_fake_fileInfo(file, path);
		file->valid = false;
		return;
	}
	file->valid = true;
		

	fill_basic_info(file, &sfile, total_size);
	fill_user_group_info(file, &sfile, exit_status);

	fill_permissions(file, &sfile);
	fill_extended_attrs(file, exit_status);

	fill_last_modified(file, &sfile, flag_label);
	fill_symlink_target(path, file, exit_status);

}

// fonction pour list_directory

void	process_argument_files(t_dyn *files,t_stack **stack, t_stack **fileList, t_flags *flag, t_exit_status *exit_status){
	while (*fileList != NULL){
		char    *file_path = pop(fileList);

		//t_fileData *file = malloc(sizeof(t_fileData));
		t_fileData *file = malloc_fileData();
		if (!file) {
			perror("malloc");
			set_exit_status(exit_status, 1, strerror(errno));
			continue;
		}
		
		file->argument = true;
		file->fileName = ft_strdup(file_path);
		file->path = getPath(file_path);
		file->absolutePath = ft_strdup(file_path);
		//ft_printf("\n {(%s0}\n", file_path);
		get_fileInfo(file_path, file, flag,&files->total_size, exit_status ); //
		if (file->fileType == 'd') {
			// Ajouter à la pile pour exploration
			push(stack, file->absolutePath);
		} else {
			// Ajouter à la liste des fichiers normaux
			append(files, file);
		}
		free(file_path);
	}
}

/* void    format_last_modified(t_fileData *file, time_t now, int i){
	printf("----%d[%s]\n",i, file->timeStr);
	if (!file->timeStr){
		file->lastModified[0] = '\0';
		return;
	}

	if (now - file->st_mtimes > 6 * 30 * 24 * 60 * 60){
		ft_strlcpy(file->lastModified, file->timeStr + 4, 8);
		ft_strlcat(file->lastModified, "  ", 20);
		ft_strlcat(file->lastModified, file->timeStr + 20, 20);

	} 
} */

// fonction pour display_sorted_files VOIR QUELLE VERSION
int	(*get_cmp_func(t_flags *flags))(t_fileData *, t_fileData *) {
	if (flags->t && !flags->r){
		//printf("\nsorting compare time [%p]\n", compare_by_time);
		return compare_by_time;}
	else if (flags->t && flags->r)
		return compare_by_time_reverse;
	else if (!flags->t && !flags->r)
		return compare_by_fileName;
	else
		return compare_by_fileName_reverse;
}

int	(*get_cmp_func1(t_flags *flags))(t_fileData *, t_fileData *) {
	if (flags->t){
		if (flags->u){
			if (flags->r){
				return cmp_atime_rev;
			}
			else{
				
				return cmp_atime;
			}
				
		} else {
			if (flags->r){
				return cmp_mtime_rev;
			}
			else{

				return compare_by_time;
			}
				
		}
	} else {
		if (flags->r){
			
			return compare_by_fileName_reverse;
		}
			
		else
		{
			
			return compare_by_fileName;
		}		
	}
}



void display_sorted_files(bool an_error,t_dyn *files, t_flags *flags, bool is_directory) {
	
	char extra ;
	char *format = ft_strdup("%-10s %3ld %-10s %10s %7ld %-20s %-30s");
	if (!flags->U){ // trie la liste des fichiers et dossier dans le repertoire courant
		
		int (*cmp)(t_fileData *, t_fileData *) = get_cmp_func(flags);
		mergeSort_iterative(files->list, files->length, cmp);
	}
	

	if (flags->l) {
		
		if (is_directory && files->length >= 0  && !an_error) {ft_printf("total %lld\n", files->total_size);} // a voir si files->length corrige les impression d'erreurs
		
		for (int i = 0; i < files->length; i++) {
			/* if (!files->list[i]->valid)
			{
				continue;
			} */
			/* if (files->list[i]->has_xattr != '?'){
				extra = files->list[i]->has_xattr;
				if (extra == ' ')
					extra = files->list[i]->has_acl;
			} */
			extra = files->list[i]->has_xattr;
			if (extra == ' ')
				extra = files->list[i]->has_acl;
			/* char perms[12]; // 11 chars + \0
			ft_memcpy(perms, files->list[i]->permission, 10);
			perms[10] = extra;
			perms[11] = '\0'; */
			// date...
			//format_last_modified(files->list[i], now, i);
			// pas encore clair
			ft_printf("%-10s", files->list[i]->permission/* , extra */); // Permissions, alignées à gauche
			ft_printf("%c", extra);  //write(1, &extra, 1);
			//ft_printf("%-11s", perms);
			ft_printf(" %3ld", files->list[i]->linkNumber); // Nombre de liens, aligné à droite
			if (!flags->g)
				ft_printf(" %-10s", files->list[i]->owner); // Nom du propriétaire, aligné à gauche
			ft_printf(" %-3s", files->list[i]->group); // Nom du groupe, aligné à gauche
			ft_printf(" %5lld", files->list[i]->fileSize); // Taille du fichier, alignée à gauche
			ft_printf(" %-10s", files->list[i]->lastModified); // Date de dernière modification, alignée à gauche
			ft_printf(" %-10s", files->list[i]->fileName); // Nom du fichier, aligné à gauche

			/* ft_printf(format "%s\t%ld\t%s\t%s\t%ld\t%s\t%s" , 
				files->list[i]->permission, files->list[i]->linkNumber, 
				files->list[i]->owner, files->list[i]->group,
				files->list[i]->fileSize, files->list[i]->lastModified, 
				files->list[i]->fileName); */
			if (files->list[i]->fileType == 'l' && files->list[i]->link_target) {
				ft_printf(" -> %s", files->list[i]->link_target);
			}
			if (flags->e && files->list[i]->acl_text){ // ici peut etre pour afficher avec l'option @
				ft_printf("\n%s\n", files->list[i]->acl_text);
			}
			if (flags->at && files->list[i]->has_xattr== '@'){
				for (int j=0;j < files->list[i]->xattr_count; j++){
					ft_printf("\n\t%s\t%zd", files->list[i]->xattrs[j].name, files->list[i]->xattrs[j].size);
				}
			}
			
			ft_printf("\n");// interligne
		}
	} else {
		char *arr[files->length];
		for (int i = 0; i < files->length; i++) {
			arr[i] = files->list[i]->fileName;
		}
		list_directory_column(arr, files->length);
	}
	ft_printf("\n"); // il faudrait un test pour savoirsi c' est le premier ou le dernier
	free(format);
}

// sert a rien
int is_root_directory(const char *path) {
	return (ft_strcmp(path, "./") == 0 || ft_strcmp(path, ".") == 0);
}
// fonction pour list_directory
void process_directory(const char *dir, t_dyn *files, t_flags *flags, t_stack **stack, bool first_dir, t_exit_status *exit_status) {
	bool an_error = list_directory_helper(dir, files, flags, stack, exit_status);
	if (!an_error && first_dir)
		ft_printf("%s:\n", dir);
	display_sorted_files(an_error, files, flags, true);
}


void    list_directory(t_flags *flags, t_stack **stack, t_stack **fileList, t_exit_status *exit_status) {
	t_dyn files;
	init_dyn(&files);

	static bool first_dir = false;
	if (!first_dir) {  
		first_dir = (*stack && (*stack)->count > 1); 
	}
	if (( *stack == NULL && *fileList == NULL )) {
		push(stack, ".");
	}
	if (( *stack != NULL && *fileList != NULL )){
		first_dir = true;
	}

	if (fileList != NULL) {
		process_argument_files(&files, stack, fileList, flags, exit_status);
		display_sorted_files(true,&files, flags, false); // a voir
		free_dyn(&files);
		init_dyn(&files);
	}

	while(*stack){
		char *current_dir = pop(stack);
		process_directory(current_dir, &files, flags, stack, first_dir, exit_status);
		/* bool an_error = list_directory_helper(current_dir, &files, flags, stack);
		if (!an_error && first_dir) {
			ft_printf("%s:\n", current_dir);  
		} */
		//display_sorted_files(an_error,&files, flags, true);

		free(current_dir);
		free_dyn(&files);
		init_dyn(&files);
		first_dir = true;
	}
	free_dyn(&files);
}

// fonction pour list_directory_helper
bool handle_dir_open_error(const char *path) {
	ft_printf_fd(1, "%s:\n", path);
	ft_printf("total 0\n");
	ft_printf_fd(2, "ft_ls: %s %s\n", path, strerror(errno));
	return true;
}

t_fileData	*create_fileData(const char *dir_path, struct dirent *entry,t_flags *flag, long *total_size, t_exit_status *exit_status){
	//t_fileData *file = malloc(sizeof(t_fileData));
	t_fileData *file = malloc_fileData();
	if (!file) return NULL;
	// a voir avec strjoin_multiple ?
	file->fileName = ft_strdup(entry->d_name);
	char *temp = ft_strjoin(dir_path,"/");
	file->path = ft_strdup(temp);
	char *fullPath = ft_strjoin(temp, file->fileName);
	file->absolutePath = ft_strdup(fullPath); 
	free(temp);
	//ft_printf("\n {%s}\n", fullPath);
	get_fileInfo(fullPath,file,flag,total_size, exit_status);
	free(fullPath);
	return file;
}

bool	handle_subdir(t_dyn *subdirs, t_fileData *file) {
	//t_fileData *subdir = malloc(sizeof(t_fileData));
	t_fileData *subdir = malloc_fileData();
	if (!subdir) return false;

	subdir->fileName = ft_strdup(file->absolutePath); // On stocke juste le chemin absolu
	subdir->owner = NULL;
	subdir->group = NULL;
	subdir->absolutePath = NULL;
	subdir->path = NULL;
	subdir->link_target = NULL;
	subdir->acl_text = NULL;
	subdir->st_mtimes = file->st_mtimes;
	subdir->st_mtimes = file->st_atimes;
	subdir->st_ino = file->st_ino;
	append(subdirs, subdir);
	return true;
}

bool    list_directory_helper(const char *path, t_dyn *files, t_flags *flags, t_stack **stack, t_exit_status *exit_status) {
	bool status;
	DIR *dir = opendir(path);
	if (!dir)
	{
		return handle_dir_open_error(path); // true si erreur
	}
		

	t_dyn subdirs;
	init_dyn(&subdirs); 
	
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (!flags->a && (ft_strcmp(entry->d_name, "..") == 0 || entry->d_name[0] == '.')) { continue;}
		t_fileData *file = create_fileData(path, entry, flags, &files->total_size, exit_status);
		if (!file){
			perror("malloc");
			closedir(dir);
			free_dyn(files);
			status = false;//false;// me demande si plutot true ici
			return status;
		}

		// Si c'est un répertoire, l'ajouter à la pile
		if (flags->bigR && file->fileType == 'd' && ft_strcmp(entry->d_name, "..") != 0 && ft_strcmp(entry->d_name, ".") != 0)
		{
			status = handle_subdir(&subdirs, file); // si ok true
		}
		append(files, file);
	}
	closedir(dir);

	if (!flags->U){
		
		int (*cmp)(t_fileData *, t_fileData *) = get_cmp_func(flags);
		
		mergeSort_iterative(subdirs.list, subdirs.length, cmp); // ici un probleme je pense on change l'ordre de for, et cmp
	}

	for (int i = subdirs.length - 1; i >= 0; i--) {
		clean_path(subdirs.list[i]->fileName);
		push(stack,  subdirs.list[i]->fileName);
	}

	free_dyn(&subdirs);
	return false; // pas d'erreur
}







int main(int argc, char *argv[]) {
	// Forcer la locale "C" pour éviter toute gestion locale spéciale
    setenv("LC_ALL", "C", 1);
	int		double_dash_position = find_double_dash(argc, argv);
	t_flags flags = {0};
	t_exit_status exit_status = {};
	if (!parse_flags(double_dash_position, argv, &flagls)){ // avant argc
		set_exit_status(&exit_status, 2, NULL);
		return exit_status.code;
	}


	t_dyn files_arg;
	init_dyn(&files_arg);
   
	t_stack *stack = NULL;
	t_stack *argument = NULL;
	


	for (int i = 1; i < double_dash_position; i++){
		if (argv[i][0] == '-'){continue;}
		process_path(&stack, &argument, &flags, argv[i], &exit_status);
	}
	for (int i = double_dash_position + 1; i < argc; i++){ // attn
		process_path(&stack, &argument, &flags, argv[i], &exit_status);
	}

	if (!argument && !stack){
		set_exit_status(&exit_status, 2, NULL);
		return exit_status.code;
	}


	list_directory(&flagls, &stack, &argument, &exit_status );
	//list_directory_iterative_new(&flagls, &stack, &argument ); //me semble foirreeux
	
	free_dyn(&files_arg);
	
	free(exit_status.message);
	return exit_status.code;
}