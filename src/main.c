#include "../libft/includes/libft.h"
#include "../includes/types.h"
#include "../includes/sort.h"
#include "../includes/dynamic_array_string.h"
#include "../includes/bonus.h"
#include "../includes/ministack.h"

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


bool    list_directory_helper(const char *path, t_dyn *files, t_flags *flags, t_stack **stack);


bool    is_directory(const char *path){
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


bool file_exists(const char *path) {
	struct stat buffer;
	return (stat(path, &buffer) == 0);
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
				else if (argv[index][position] == 'f') flags->f = true;
				else if (argv[index][position] == 'g') flags->g = true;
				else if (argv[index][position] == 'd') flags->d = true;
				else if (argv[index][position] == 'e') flags->e = true; //bonus acl
				else {
					errno = EINVAL;
					ft_printf_fd(2,"ls: invalid option -- '%c'\n", argv[index][position]);
					ft_printf_fd(2,"ft_ls error: %s\n", strerror(errno));
					ft_printf_fd(2, "usage: ls [-Ralrtufgde] [file ...]\n");
					return false; //voir pour les erreur et leurs gestion
				}
				position++;
			}
		}
		else if (argv[index][0] == '-' && argv[index][1] == '\0'){
			errno = EINVAL;
			ft_printf_fd(2, "ls: invalid option -- '-'\n");
			ft_printf_fd(2,"ft_ls error: %s\n", strerror(errno));
			ft_printf_fd(2, "usage: ls [-Ralrtufgde] [file ...]\n");
			return false;
		}
		index++;
	}
	return true;
}


void    print_flag(t_flags *flags){
	ft_printf("R[%d] r[%d] a[%d] l[%d] t[%d] u[%d] f[%d] g[%d] d[%d] e[%d]", 
		flags->bigR,
		flags->r,
		flags->a,
		flags->l,
		flags->t,
		flags->u,
		flags->f,
		flags->g,
		flags->d,
		flags->e

	);
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
	//printf("width = %d, cols = %d, rows = %d\n", t->col_width, cols, rows);
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



long long    get_bloc_size(const char *path, const char *fileName){
	struct stat fileStat;

	char *temp = ft_strjoin(path,"/");
	char *temp1 = ft_strjoin(temp, fileName); 
	free(temp);

	if (lstat(temp1, &fileStat) == -1){
		perror("lstat");
	}
	free(temp1);
	return fileStat.st_blocks;
}

void	get_owner_group(struct stat *sfile, char **owner, char **group){
	struct passwd *pwd = getpwuid(sfile->st_uid);
	struct group *grp = getgrgid(sfile->st_gid);

	if (pwd) *owner = ft_strdup( pwd->pw_name);
		else *owner = ft_strdup("UNKNOWN");

	if (grp) *group = ft_strdup(grp->gr_name);
		else *group = ft_strdup("UNKNOWN");
}

char    get_file_type(mode_t mode){
	if (S_ISDIR(mode)) return 'd'; // directory
	if (S_ISLNK(mode)) return 'l'; // symbolic link
	if (S_ISREG(mode)) return '-'; // regular
	if (S_ISCHR(mode)) return 'c'; // character special
	if (S_ISBLK(mode)) return 'b'; // block special
	if (S_ISFIFO(mode)) return 'p'; // FIFO special
	if (S_ISSOCK(mode)) return 's'; // Socket
	return '?';
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
}
void    get_symlink_target(const char *path, char **link_target){
	char buffer[PATH_MAX];
	ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);

	if (len != -1){
		buffer[len] = '\0';
		*link_target = ft_strdup(buffer);   // leak possible        
	} else {
		*link_target = NULL;        
	} 
}
// fonction pour get_fileInfo
bool	fill_stat_data(const char *path, struct stat *sfile){
	if (lstat(path, sfile) == -1) {
		perror("lstat");
		return false;
	}
	return true;
}

void	fill_basic_info(t_fileData *file, struct stat *sfile, long *total_size){
	file->fileSize = sfile->st_size;
	file->linkNumber = sfile->st_nlink;
	file->blocSize = sfile->st_blocks;
	file->st_mtimes = sfile->st_mtime;
	file->st_ino = sfile->st_ino;

	*total_size += file->blocSize;
}

void	fill_user_group_info(t_fileData *file, struct stat *sfile){
	get_owner_group(sfile, &file->owner, &file->group);	
}

void fill_permissions(t_fileData *file, struct stat *sfile){
	file->fileType =get_file_type(sfile->st_mode);
	file->permission[0] = file->fileType;
	get_permissions(sfile->st_mode, file->permission);
}

void fill_extended_attrs(t_fileData *file){
	char *tmp = NULL;
	file->acl_text = NULL;
	file->has_xattr= has_xattr(file->absolutePath);
	if (file->has_xattr == ' '){
		file->has_acl = has_acl(file->absolutePath, &tmp);
		if (file->has_acl == '+')
		{
			file->acl_text = format_acl_text(tmp);
			free(tmp);
		}
	}
}

void fill_last_modified(t_fileData *file, struct stat *sfile){
	time_t	now = time(NULL);
	char    *timeStr = ctime(&sfile->st_mtime);
	if (timeStr){
		timeStr[ft_strlen(timeStr) - 1] = '\0'; 
		if (now - sfile->st_mtime > 6 * 30 * 24 * 60 * 60) {// 15552000 je crois
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

void fill_symlink_target(const char *path, t_fileData *file) {
	if (file->fileType == 'l')
		get_symlink_target(path, &file->link_target);
	else
		file->link_target = NULL;
}

void    get_fileInfo(const char* path, t_fileData *file, long *total_size){
	struct stat sfile;

	if (!fill_stat_data(path, &sfile))
		return;

	fill_basic_info(file, &sfile, total_size);
	fill_user_group_info(file, &sfile);
	fill_permissions(file, &sfile);
	fill_extended_attrs(file);
	fill_last_modified(file, &sfile);
	fill_symlink_target(path, file);
}


void    process_argument_files(t_dyn *files,t_stack **stack, t_stack **fileList){
	while (*fileList != NULL){
		char    *file_path = pop(fileList);

		t_fileData *file = malloc(sizeof(t_fileData));
		if (!file) {
			perror("malloc");
			continue;
		}
		
		file->argument = true;
		file->fileName = ft_strdup(file_path);
		file->path = getPath(file_path);
		file->absolutePath = ft_strdup(file_path);

		get_fileInfo(file_path, file, &files->total_size );
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

void    format_last_modified(t_fileData *file, time_t now, int i){
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
}

// fonction pour display_sorted_files
int	(*get_cmp_func(t_flags *flags))(t_fileData *, t_fileData *) {
	if (flags->t && !flags->r)
		return compare_by_time;
	else if (flags->t && flags->r)
		return compare_by_time_reverse;
	else if (!flags->t && !flags->r)
		return compare_by_fileName;
	else
		return compare_by_fileName_reverse;
}


void display_sorted_files(bool an_error,t_dyn *files, t_flags *flags, bool is_directory) {
	//int (*cmp)(t_fileData *, t_fileData *) = NULL;
	char extra = ' ';
	char *format = ft_strdup("%-10s %3ld %-10s %10s %7ld %-20s %-30s");

	int (*cmp)(t_fileData *, t_fileData *) = get_cmp_func(flags);
	mergeSort_iterative(files->list, files->length, cmp);

	if (flags->l) {
		
		if (is_directory && files->length >= 0  && !an_error) {ft_printf("total %lld\n", files->total_size);} // a voir si files->length corrige les impression d'erreurs
		
		for (int i = 0; i < files->length; i++) {
			extra = files->list[i]->has_xattr;
			if (extra == ' ')
				extra = files->list[i]->has_acl;
			/* extra = has_xattr(files->list[i]->absolutePath);
			if (extra == ' ')  // Si pas d'attributs étendus, on vérifie les ACLs
				extra = has_acl(files->list[i]->absolutePath); */
			// date...
			//format_last_modified(files->list[i], now, i);
			// pas encore clair
			ft_printf("%-10s%c", files->list[i]->permission, extra); // Permissions, alignées à gauche
			ft_printf(" %3ld", files->list[i]->linkNumber); // Nombre de liens, aligné à droite
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
			if (flags->e && files->list[i]->acl_text){
				ft_printf("\n%s\n", files->list[i]->acl_text);
			}

			ft_printf("\n");
		}
	} else {
		char *arr[files->length];
		for (int i = 0; i < files->length; i++) {
			arr[i] = files->list[i]->fileName;
		}
		list_directory_column(arr, files->length);
	}
	ft_printf("\n");
	free(format);
}

// sert a rien
int is_root_directory(const char *path) {
	return (strcmp(path, "./") == 0 || strcmp(path, ".") == 0);
}
// fonction pour list_directory
void process_directory(const char *dir, t_dyn *files, t_flags *flags, t_stack **stack, bool first_dir) {
	bool an_error = list_directory_helper(dir, files, flags, stack);
	if (!an_error && first_dir)
		ft_printf("%s:\n", dir);
	display_sorted_files(an_error, files, flags, true);
}


void    list_directory(t_flags *flags, t_stack **stack, t_stack **fileList) {
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
		process_argument_files(&files, stack, fileList);
		display_sorted_files(true,&files, flags, false); // a voir
		free_dyn(&files);
		init_dyn(&files);
	}

	while(*stack){
		char *current_dir = pop(stack);
		process_directory(current_dir, &files, flags, stack, first_dir);
		/* bool an_error = list_directory_helper(current_dir, &files, flags, stack);
		if (!an_error && first_dir) {
			ft_printf("%s:\n", current_dir);  
		}
		display_sorted_files(an_error,&files, flags, true); */

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

t_fileData	*create_fileData(const char *dir_path, struct dirent *entry, long *total_size){
	t_fileData *file = malloc(sizeof(t_fileData));
	if (!file) return NULL;
	// a voir avec strjoin_multiple ?
	file->fileName = ft_strdup(entry->d_name);
	char *temp = ft_strjoin(dir_path,"/");
	file->path = ft_strdup(temp);
	char *fullPath = ft_strjoin(temp, file->fileName);
	file->absolutePath = ft_strdup(fullPath); 
	free(temp);
	get_fileInfo(fullPath,file,total_size);
	free(fullPath);
	return file;
}

void	handle_subdir(t_dyn *subdirs, t_fileData *file/* , struct dirent *entry */) {
	t_fileData *subdir = malloc(sizeof(t_fileData));
	if (!subdir) return;

	subdir->fileName = ft_strdup(file->absolutePath); // On stocke juste le chemin absolu
	subdir->owner = NULL;
	subdir->group = NULL;
	subdir->absolutePath = NULL;
	subdir->path = NULL;
	subdir->link_target = NULL;
	subdir->acl_text = NULL;
	append(subdirs, subdir);
}

bool    list_directory_helper(const char *path, t_dyn *files, t_flags *flags, t_stack **stack) {
	DIR *dir = opendir(path);
	if (!dir)
		return handle_dir_open_error(path);

	t_dyn subdirs;
	init_dyn(&subdirs); 
	
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (!flags->a && (ft_strcmp(entry->d_name, "..") == 0 || entry->d_name[0] == '.')) { continue;}
		t_fileData *file = create_fileData(path, entry, &files->total_size);
		if (!file){
			perror("malloc");
			closedir(dir);
			free_dyn(files);
			return false;//
		}

		// Si c'est un répertoire, l'ajouter à la pile
		if (flags->bigR && file->fileType == 'd' && ft_strcmp(entry->d_name, "..") != 0 && ft_strcmp(entry->d_name, ".") != 0)
		{
			handle_subdir(&subdirs, file);
		}
		append(files, file);
	}
	closedir(dir);
	mergeSort_iterative(subdirs.list, subdirs.length, compare_by_fileName_reverse);
	for (int i= 0;i < subdirs.length; i++){
		clean_path(subdirs.list[i]->fileName);
		push(stack,  subdirs.list[i]->fileName);
	}
	free_dyn(&subdirs);
	return false;//
}

// pas encore operationnelle foireux
void    list_directory_iterative_new(t_flags *flags, t_stack **stack, t_stack **fileList){
	t_stack *dummy_stack = NULL;
	
	if (fileList && *fileList) {
		printf("Processing (iterative) argument files...\n");
		list_directory(flags, &dummy_stack, fileList);
	}

	// 2. Si stack est vide, ajouter "." pour traiter le répertoire courant
	if (!(*stack)) {
		push(stack, ".");
	}

	list_directory(flags, stack, NULL);

	//print_stack(*stack);
	// !!!! pourquoi ?
   /* while (*stack != NULL){
		char *current_dir = pop(stack);
		printf("**current dir %s\n", current_dir);
		DIR *dir = opendir(current_dir);  
		
		
		if (!dir){ // meilleures version

			print_relative_path(1,current_dir, current_dir); ft_printf("\n");
			//ft_printf("a%s:\n", current_dir); // a tester // Toujours afficher le nom du répertoire
			ft_printf("total 0\n"); // a tester
			ft_printf_fd(2, "\nls: ");
			print_relative_path(2,current_dir, current_dir);
			ft_printf_fd(2," %s\n", strerror(errno) );
			//ft_printf_fd(2,"\nls: %s: %s\n", current_dir, strerror(errno) );
			free(current_dir);
			continue;
		}
	   
		if (flags->bigR && ft_strcmp(current_dir,current_dir) != 0){
			
			
			print_relative_path(1,current_dir, current_dir);
			ft_printf("\n");
			
		}
		list_directory( flags, stack, NULL);

		closedir(dir);
		free(current_dir);
	}*/
	//free_dyn(&files);
}


void    list_directory_iterative_backup(const char *start_path,  t_flags *flags){
	t_stack *stack = NULL;  // Pile des répertoires à explorer
	
	//push(&stack, start_path);  // Ajouter le répertoire de départ à la pile


	list_directory(/* start_path, */flags, &stack, NULL); // ici NULL pour la liste des fichier mais a voir c'est temporaire
	//print_stack(stack);

	
	while (stack != NULL){
		char *current_dir = pop(&stack);  // Récupérer le répertoire à explorer
		DIR *dir = opendir(current_dir);  // Ouvrir le répertoire
		//printf("**start directory {%s}\tcurrent directory [%s] \n", start_path, current_dir);
		//print_stack(stack);
		if (!dir){ // meilleures version
			print_relative_path(1,current_dir, start_path); ft_printf("\n");
			//ft_printf("a%s:\n", current_dir); // a tester // Toujours afficher le nom du répertoire
			ft_printf("total 0\n"); // a tester
			ft_printf_fd(2, "\nls: ");
			print_relative_path(2,current_dir, start_path);
			ft_printf_fd(2," %s\n", strerror(errno) );
			//ft_printf_fd(2,"\nls: %s: %s\n", current_dir, strerror(errno) );
			free(current_dir);
			continue;
		}
		//printf("--start directory {%s}\tcurrent directory [%s] \n", start_path, current_dir);
		if (flags->bigR && ft_strcmp(current_dir,start_path) != 0){
			
			
			print_relative_path(1,current_dir, start_path);
			ft_printf("\n");
			
		} // {ft_printf("%s\n",current_dir);} // ici il n' affiche pas correctement le directory !
		list_directory(/* current_dir, */ flags, &stack, NULL); // ici NULL pour la liste des fichier mais a voir c'est temporaire

		closedir(dir);
		free(current_dir);
	}
	//free_dyn(&files);
}


int main(int argc, char *argv[]) {
	t_flags flagls = {0};
	bool had_minor_error = false;
	bool had_serious_error = ! parse_flags(argc, argv, &flagls);

	if (had_serious_error){
		ft_printf_fd(2, "Serious error detected. Returning 2.\n");
		return 2;
	}
	t_dyn files_arg;
	init_dyn(&files_arg);
   
	t_stack *stack = NULL;
	t_stack *argument = NULL;
	


	for (int i= 1; i < argc; i++){
		if (argv[i][0] == '-') continue; 
		
		if (is_directory(argv[i])){
			push(&stack, argv[i]);
			
		}
		else if (file_exists(argv[i])){
			push(&argument, argv[i]);

		} else {
			struct stat buffer;
	
			if (stat(argv[i], &buffer) != 0) {
				had_minor_error = true;
				ft_printf_fd(2,"\nft_ls!: %s: %s\n", argv[i], strerror(errno));
			}
		}
	}

	list_directory(&flagls, &stack, &argument );
	//list_directory_iterative_new(&flagls, &stack, &argument ); //me semble foirreeux
	
	free_dyn(&files_arg);
	if (had_minor_error){
		ft_printf_fd(2, "Minor error detected. Returning 1.\n");
		return 1;
	}
		
	return 0;
}