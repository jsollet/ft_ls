#include "../libft/includes/libft.h"
#include "../includes/types.h"
#include "../includes/sort.h"
#include "../includes/dynamic_array_string.h"
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
//#include <sys/ioctl.h>

#define MAX_PATH_LEN 1024

// Structure de la pile pour les répertoires à explorer
typedef struct s_stack {
    char *path;
    struct s_stack *next;
} t_stack;

void    list_directory_helper(const char *path, t_dyn *files, t_flags *flags, t_stack **stack);

// Fonction pour empiler un chemin dans la pile
void push(t_stack **stack, const char *path) {
    t_stack *new_node = malloc(sizeof(t_stack));
    if (!new_node) {
        perror("malloc");
        return;
    }
    new_node->path = ft_strdup(path);
    new_node->next = *stack;
    *stack = new_node;
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

// GPT pour imprimer correctement ?? 
void print_relative_path(int fd,const char *full_path, const char *start_path) {
    if (ft_strncmp(full_path, start_path, ft_strlen(start_path)) == 0) {
        full_path += ft_strlen(start_path); // Ignore le préfixe start_path
        while (*full_path == '/') full_path++; // Évite les `/` superflus
    }
    ft_printf_fd(fd,"./%s:", full_path);
}
/* void push(t_stack **stack, const char *path) {
    if (path == NULL) {
        printf("Error: NULL path in push\n");
        return;
    }
    t_stack *new_node = malloc(sizeof(t_stack));
    if (new_node == NULL) {
        printf("Error: malloc failed in push\n");
        return;
    }
    new_node->path = strdup(path);
    if (new_node->path == NULL) {
        printf("Error: strdup failed in push\n");
        free(new_node);
        return;
    }
    new_node->next = *stack;
    *stack = new_node;
} */

// Fonction pour dépiler un chemin de la pile
char *pop(t_stack **stack) {
    if (*stack == NULL) return NULL;
    t_stack *top = *stack;
    *stack = top->next;
    char *path = top->path;
    free(top);
    return path;
}

void print_stack(t_stack *stack) {
    printf("Path: ");
    while (stack != NULL) {
        printf("%s ->", stack->path);
        stack = stack->next;
    }
    printf("\n");
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
    int index = 0;
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
                else {
                    errno = EINVAL;
                    ft_printf_fd(2,"ls: invalid option -- '%c'\n", argv[index][position]);
                    ft_printf_fd(2,"ft_ls error: %s\n", strerror(errno));
                    ft_printf_fd(2, "usage: ls [-Ralrtufgd] [file ...]\n");
                    return false; //voir pour les erreur et leurs gestion
                }
                position++;
            }
        }
        else if (argv[index][0] == '-' && argv[index][1] == '\0'){
            errno = EINVAL;
            ft_printf_fd(2, "ls: invalid option -- '-'\n");
            ft_printf_fd(2,"ft_ls error: %s\n", strerror(errno));
            ft_printf_fd(2, "usage: ls [-Ralrtufgd] [file ...]\n");
            return false;
            return false; //?
        }
        index++;
    }
    return true;
}


void    print_flag(t_flags *flags){
    ft_printf("R[%d] r[%d] a[%d] l[%d] t[%d] u[%d] f[%d] g[%d] d[%d]", 
        flags->bigR,
        flags->r,
        flags->a,
        flags->l,
        flags->t,
        flags->u,
        flags->f,
        flags->g,
        flags->d
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
void    list_directory_column(/* const char *path, */ char *arr[], int arr_length){
    t_term t;
    init_terminal(&t);
    update_terminal(&t, arr, arr_length);
    print_files_in_columns(arr, &t);
}



long long    get_bloc_size(const char *path, const char *fileName){
    struct stat fileStat;

    // construit le chemin complet
    //char full_path[MAX_PATH_LEN];
    char *temp = ft_strjoin(path,"/");
    char *temp1 = ft_strjoin(temp, fileName); 
    free(temp);

    if (lstat(temp1, &fileStat) == -1){
        perror("lstat");
    }
    free(temp1);
    return fileStat.st_blocks;
}

void    get_owner_group(struct stat *sfile, char **owner, char **group){
    struct passwd *pwd = getpwuid(sfile->st_uid);
    struct group *grp = getgrgid(sfile->st_gid);

    if (pwd) *owner = ft_strdup( pwd->pw_name);
        else *owner = ft_strdup("UNKNOWN");

        if (grp) *group = ft_strdup(grp->gr_name);
        else *group = ft_strdup("UNKNOWN");
}

void    get_fileInfo(const char* path, t_fileData *file, long *total_size){
    struct stat sfile;
    if (lstat(path, &sfile) == -1)
    {
        perror("lstat");
        return;
    }

    file->link_target = NULL;
    file->fileSize = sfile.st_size;
    file->linkNumber = sfile.st_nlink;
    file->blocSize = sfile.st_blocks;

    file->st_mtimes = sfile.st_mtime; // pour le temps en secondes
    file->st_ino = sfile.st_ino;

    *total_size += file->blocSize; // calcul du total des fichiers

    get_owner_group(&sfile, &file->owner, &file->group);
    /* struct passwd *pwd = getpwuid(sfile.st_uid); // ce sont des structures
    if (pwd) file->owner = ft_strdup( pwd->pw_name);
        else file->owner = ft_strdup("UNKNOWN");

    struct group *grp = getgrgid(sfile.st_gid); // ce sont des structures
    if (grp) file->group = ft_strdup(grp->gr_name);
    else file->group = ft_strdup("UNKNOWN"); */

    

    // permission a voir pour mettre dans une autre fonction
    if ((sfile.st_mode & S_IFMT) == S_IFDIR) file->permission[0] = 'd'; // a completer pour les types de fichier
        else if ((sfile.st_mode & S_IFMT) == S_IFLNK){file->permission[0] = 'l';} 
        else file->permission[0] = '-';

    if (sfile.st_mode & S_IRUSR) file->permission[1] = 'r';
        else file->permission[1] = '-';
    if (sfile.st_mode & S_IWUSR) file->permission[2] = 'w';
        else file->permission[2] = '-';
    if (sfile.st_mode & S_IXUSR) file->permission[3] = 'x';
        else file->permission[3] = '-';
    if (sfile.st_mode & S_IRGRP) file->permission[4] = 'r';
        else file->permission[4] = '-';
    if (sfile.st_mode & S_IWGRP) file->permission[5] = 'w';
        else file->permission[5] = '-';
    if (sfile.st_mode & S_IXGRP) file->permission[6] = 'x';
        else file->permission[6] = '-';
    if (sfile.st_mode & S_IROTH) file->permission[7] = 'r';
        else file->permission[7] = '-';
    if (sfile.st_mode & S_IWOTH) file->permission[8] = 'w';
        else file->permission[8] = '-';
    if (sfile.st_mode & S_IXOTH) file->permission[9] = 'x';
        else file->permission[9] = '-';
    file->permission[10] = '\0';
    
    // date de modification // a voir
    char    *timeStr = ctime(&sfile.st_mtime);
    if (timeStr){
        ft_strlcpy(file->lastModified,timeStr,ft_strlen(timeStr)-5);
    } else {
        file->lastModified[0] = '\0';
    }
    
    // type de fichier
    if ((sfile.st_mode & S_IFMT) == S_IFDIR) file->fileType = 'd';
    else if ((sfile.st_mode & S_IFMT) == S_IFLNK) {
        file->fileType = 'l';

        char link_target[PATH_MAX];
        ssize_t len = readlink(path, link_target, sizeof(link_target) - 1);

        if (len != -1){
            link_target[len] = '\0';
            file->link_target = ft_strdup(link_target);   // leak possible
            
        } else {
            file->link_target = NULL;
            
        }
        //printf("filetype = %c, target %s\n", file->fileType, file->link_target);
    } 
    else file->fileType = '-';
}



void    list_directory(const char *path, t_flags *flags, t_stack **stack) {
    t_dyn files;
    init_dyn(&files);

   /*  DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");    
        return;
    } */
    
    //printf("Allocated fileData for *dir [%p]\n", dir);
    /* struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!flags->a && (ft_strcmp(entry->d_name, "..") == 0 || entry->d_name[0] == '.')) { continue;}
        

        t_fileData *file = malloc(sizeof(t_fileData));
        if (!file){
            perror("malloc");
            closedir(dir);
            free_dyn(&files);
            return;
        }

        file->fileName = ft_strdup(entry->d_name);

        char *temp = ft_strjoin(path,"/");
        file->path = ft_strdup(temp);
        char *fullPath = ft_strjoin(temp, file->fileName);
        file->absolutePath = ft_strdup(fullPath); 
        free(temp);
        get_fileInfo(fullPath,file,&files.total_size);
        free(fullPath);
        append(&files, file);
    }
    
    closedir(dir); */
    
    list_directory_helper(path, &files, flags, stack); // test normalisation
    
    // ici le tri
    int (*cmp)(t_fileData *, t_fileData *) = NULL;
    if (flags->t && !flags->r)
        cmp = compare_by_time;
    else if (flags->t && flags->r)
        cmp = compare_by_time_reverse;
    else if (!flags->t && !flags->r)
    {
        cmp = compare_by_fileName;
        
    }
    else if (!flags->t && flags->r)
        cmp = compare_by_fileName_reverse;   
    
        

    mergeSort_iterative(files.list, files.length, cmp);
    // Affichage des fichiers (ou autre traitement)cela depend des flags
    
    if (flags->l){
        
        ft_printf("Total %lld\n", files.total_size);
        for (int i = 0; i < files.length; i++) {
            ft_printf("%s\t%ld\t%s\t%s\t%ld\t%s\t%s", files.list[i]->permission, files.list[i]->linkNumber, files.list[i]->owner, files.list[i]->group,
                files.list[i]->fileSize, files.list[i]->lastModified, files.list[i]->fileName);
                if (files.list[i]->fileType == 'l' && files.list[i]->link_target) {
                    ft_printf(" -> %s", files.list[i]->link_target);  // Ajoute la cible du lien
                }
                ft_printf("\n");
        }
        ft_printf("\n");
    } else {
        char *arr[files.length];
        for (int i = 0; i < files.length; i++) {
            arr[i] = files.list[i]->fileName; // Assumer que fileName est une chaîne allouée et valide
        }
        list_directory_column(arr, files.length);
        printf("\n"); // ici pour ameliorer printing de -R 
    }
    free_dyn(&files);
}

void    list_directory_helper(const char *path, t_dyn *files, t_flags *flags, t_stack **stack) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");    
        return;
    }
    //bool    has_subdir = false;
    
    struct dirent *entry;
    // tri des subdirs
    t_dyn subdirs;
    init_dyn(&subdirs); 
    while ((entry = readdir(dir)) != NULL) {
        if (!flags->a && (ft_strcmp(entry->d_name, "..") == 0 || entry->d_name[0] == '.')) { continue;}
        
        t_fileData *file = malloc(sizeof(t_fileData));
        if (!file){
            perror("malloc");
            closedir(dir);
            free_dyn(files);
            return;
        }

        
        
        file->fileName = ft_strdup(entry->d_name);
        char *temp = ft_strjoin(path,"/");
        file->path = ft_strdup(temp);
        char *fullPath = ft_strjoin(temp, file->fileName);
        file->absolutePath = ft_strdup(fullPath); 
        free(temp);
        get_fileInfo(fullPath,file,&files->total_size);

        // Si c'est un répertoire, l'ajouter à la pile
        if (flags->bigR && file->fileType == 'd' && ft_strcmp(entry->d_name, "..") != 0 && ft_strcmp(entry->d_name, ".") != 0)
        { // ici a retourner vers
            t_fileData *subdir = malloc(sizeof(t_fileData)); // LEAK si pas de subdir on aura un probleme avec le free plus loin ?
            if (!file){
                perror("malloc");
                closedir(dir);
                free_dyn(&subdirs);
                return;
            }
            //has_subdir = true;
            
            subdir->owner = NULL;
            subdir->group = NULL;
            subdir->absolutePath = NULL;
            subdir->fileName = ft_strdup(fullPath);// avant fullpath
            
            subdir->path = NULL;
            subdir->link_target = NULL;
            append(&subdirs, subdir); // LEAK faux
            //free(subdir);
            //push(stack, fullPath);  // Ajouter à la pile pour exploration
            //printf("--");print_stack(*stack);
        }

        //free(subdir);
        free(fullPath);
        append(files, file);
        
    }
    closedir(dir);
    // tri a la fin
    // a tester sur un repertoire vide
    mergeSort_iterative(subdirs.list, subdirs.length, compare_by_fileName_reverse);
    // et envoi a la pile
    for (int i= 0;i < subdirs.length; i++){
        clean_path(subdirs.list[i]->fileName); // test pour enlever les "//"
        push(stack,  subdirs.list[i]->fileName);
    }
    free_dyn(&subdirs); 
}

// pas encore operationnelle
void    list_directory_iterative_new(const char *start_path,  t_flags *flags){
    t_stack *stack = NULL;  // Pile des répertoires à explorer
    //push(&stack, start_path);  // Ajouter le répertoire de départ à la pile
    
    

    printf("\033[0;32m");
    
    list_directory(start_path, flags, &stack);
    //print_stack(stack);
    printf("\033[0m\n");
    
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
        list_directory(current_dir, flags, &stack);

        closedir(dir);
        free(current_dir);
    }
    //free_dyn(&files);
}

















// Fonction pour lister les fichiers d'un répertoire de manière itérative
void    list_directory_iterative(const char *start_path) {
    t_stack *stack = NULL;  // Pile des répertoires à explorer
    push(&stack, start_path);  // Ajouter le répertoire de départ à la pile

    while (stack != NULL) {
        char *current_dir = pop(&stack);  // Récupérer le répertoire à explorer
        DIR *dir = opendir(current_dir);  // Ouvrir le répertoire

        if (dir == NULL) {
            perror("opendir");
            free(current_dir);
            continue;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            // Ignorer . et .. pour ne pas revenir au répertoire parent
            if (ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            // Afficher le fichier
            char full_path[MAX_PATH_LEN];
            snprintf(full_path, sizeof(full_path), "%s/%s", current_dir, entry->d_name);
            ft_printf("%s\n", full_path);

            // Si c'est un répertoire, ajouter à la pile pour exploration ultérieure
            if (entry->d_type == DT_DIR) {
                push(&stack, full_path);
            }
        }

        closedir(dir);
        free(current_dir);  // Libérer le chemin après utilisation
    }
}

int main(int argc, char *argv[]) {
    t_flags flagls = {0};
    bool result = parse_flags(argc, argv, &flagls);

    if (result) {;}
    check_files_exist(argc, argv);
    //print_flag(&flagls);

    const char *start_path = "./test_env/";  // Exemple de répertoire de départ
    //list_directory_iterative(start_path);

    //flagls.bigR = true;
    //list_directory(start_path, &flagls, NULL);//peut foirer
    list_directory_iterative_new(start_path, &flagls);
    
    return 0;
}