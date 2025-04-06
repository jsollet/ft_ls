#ifndef TYPES_H
# define TYPES_H
# include <stddef.h>
# include <stdbool.h>
# include <time.h>
# include <sys/stat.h>
#include <sys/ioctl.h>

#define MAX_PATH_LEN 1024

// Structure de la pile pour les répertoires à explorer
typedef struct s_stack {
	size_t count;
	char *path;
	struct s_stack *next;
} t_stack;

typedef struct s_flags
{
    bool    bigR;
    bool    r;
    bool    a;
    bool    l;
    bool    t;

    // bonus a voir...
    bool    e; // pour les acl
    bool    u;
    bool    f;
    bool    g;
    bool    d;

}   t_flags;

typedef struct s_fileData
{
    char*           fileName;
    char*           absolutePath;
    char*           path;
    char*           owner;
    char*           group;
    char*           link_target;
    char*           timeStr;
    char*           acl_text;

    long long       fileSize;
    long            linkNumber;
    long long       blocSize;
    //long long       total_size; // total de l'espace occupé par les fichiers dans le répertoire

    bool            argument;
    
    char            fileType;
    char            permission[11]; // Permissions (ex: "-rw-r--r--")
    char            lastModified[20]; // Date de modification (format "Feb 21 14:22")

    char            has_acl;
    char            has_xattr;

    time_t          st_mtimes;
    ino_t           st_ino;

}   t_fileData;

typedef struct s_term
{
    struct winsize  w;
    int             term_width;
    int             col_width;
    size_t          count;
    size_t          max_len;
}   t_term;

#endif