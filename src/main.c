#include "../libft/includes/libft.h"
#include "../includes/types.h"
#include "../includes/sort.h"
#include "../includes/dynamic_array_string.h"
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
//#include <sys/ioctl.h>

#define MAX_PATH_LEN 1024

// Structure de la pile pour les répertoires à explorer
typedef struct s_stack {
    char *path;
    struct s_stack *next;
} t_stack;

// Fonction pour empiler un chemin dans la pile
void push(t_stack **stack, const char *path) {
    t_stack *new_node = malloc(sizeof(t_stack));
    new_node->path = strdup(path);
    new_node->next = *stack;
    *stack = new_node;
}

// Fonction pour dépiler un chemin de la pile
char *pop(t_stack **stack) {
    if (*stack == NULL) return NULL;
    t_stack *top = *stack;
    *stack = top->next;
    char *path = top->path;
    free(top);
    return path;
}


void check_files_exist(int argc, char *argv[]){
    struct stat buffer;
    int index = 1;
    while (index < argc){
        if (argv[index][0] == '-') continue;

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
            return false;
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
    t->col_width = t->max_len + 2;  // Ajout d'un espace pour la lisibilité
}

/* void    print_files_in_columns(char **files, t_term *t) {
    int cols = t->term_width / t->col_width;
    if (cols == 0) cols = 1;  // Sécurité pour éviter une division par zéro

    for (size_t i = 0; i < t->count; i++) {
        printf("%-*s", t->col_width, files[i]);  // Alignement gauche
        if ((i + 1) % cols == 0)
            ft_printf("\n");
    }
    ft_printf("\n");
} */

void    print_files_in_columns(char **files, t_term *t) {
    int cols = t->term_width / t->col_width;
    if (cols == 0) cols = 1;  // Sécurité pour éviter une division par zéro

    for (size_t i = 0; i < t->count; i++) {
        // Afficher la chaîne (alignement à gauche)
        write(1, files[i], strlen(files[i]));
        
        // Ajouter des espaces pour obtenir l'alignement à gauche
        int space_count = t->col_width - strlen(files[i]);
        for (int j = 0; j < space_count; j++) {
            write(1, " ", 1);
        }

        // Ajouter une nouvelle ligne après chaque colonne, si nécessaire
        if ((i + 1) % cols == 0) {
            write(1, "\n", 1);
        }
    }
    write(1, "\n", 1);  // Nouvelle ligne à la fin
}
// a continuer
void    list_directory_column(const char *path){
    t_term t;
    init_terminal(&t);

    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }
    struct dirent *entry;
    t_dyn fileList;
    init_dyn(&fileList);

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue; // Ignore fichiers cachés (comme `ls` normal)
        append(&fileList, entry->d_name);
        /* size_t len = ft_strlen(entry->d_name);
        if (len > t.max_len){
            t.max_len = len;
        }
        t.count++; */
    }
    closedir(dir);

    if (fileList.length == 0){
        free_dyn(&fileList);
        return;
    }

    update_terminal(&t, fileList.list, fileList.length);
    print_files_in_columns(fileList.list, &t);

    /* t.col_width = t.max_len + 2;
    int cols = t.term_width / t.col_width;
    if (cols < 1) cols = 1;

    for (size_t i = 0; i < t.count; i++) {
        ft_printf("%-*s", t.col_width, fileList.list[i]); // Alignement
        if ((i + 1) % cols == 0 || i == t.count - 1) ft_printf("\n");
        
    } */
    free_dyn(&fileList);
}


void    list_directory(const char *path) {
    
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");    
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Ignorer . et .. pour ne pas revenir au répertoire parent
        if (ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        char full_path[MAX_PATH_LEN];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        //ft_printf("%s\n", full_path);
        ft_printf("%s\n", entry->d_name);
        }

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
    ft_printf("parsing is %s\n", result ? "true":"false");
    for (int i = 0; i < argc; i++) {
        ft_printf("Argument %d: %s\n", i, argv[i]);
    }
    print_flag(&flagls);
    check_files_exist(argc, argv);


    char *filenames[] = {"file3.txt", "file1.txt", "file2.txt", "file4.txt"};
    int n = sizeof(filenames) / sizeof(filenames[0]);

    mergeSort_iterative(filenames, n);

    for (int i = 0; i < n; i++)
        ft_printf("%s\n", filenames[i]);

    
    const char *start_path = ".";  // Exemple de répertoire de départ
    list_directory_iterative(start_path);
    printf("\n\n");
    list_directory(start_path);
    printf("printf alignement gauche\n\n");
    printf("[%-10s] [%s]\n", "test", "alignement");
    printf("ft_printf alignement gauche\n\n");
    ft_printf("[%-10s] [%s]\n", "test", "alignement");
    printf("\n\n");
    list_directory_column(start_path);
    return 0;
}