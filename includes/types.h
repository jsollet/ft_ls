#ifndef TYPES_H
# define TYPES_H
# include <stddef.h>
# include <stdbool.h>
# include <time.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/ioctl.h>

#define MAX_PATH_LEN 1024




// Structure de la pile pour les répertoires à explorer
typedef struct s_stack
{
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
    bool    at; // @ pour attibut etendu
    bool    u;
    bool    U; // pour f
    bool    f;
    bool    g;
    bool    d;

}   t_flags;

typedef struct s_xttr
{
    char *name;
    ssize_t size;
} t_attr;

typedef struct s_special_bit {
	mode_t bit;
	mode_t exec;
	char   set;
	char   no_exec;
	int    pos;
}	t_special_bit;


typedef struct s_fileData
{
    bool            valid;        
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

    bool            argument;
    
    char            fileType;
    char            permission[11]; // Permissions (ex: "-rw-r--r--")
    char            lastModified[20]; // Date de modification (format "Feb 21 14:22")

    t_attr          *xattrs;
    int             xattr_count;
    bool            print_xattrs;

    char            has_acl;
    char            has_xattr;

    time_t          st_mtimes;
    time_t          st_atimes;
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

typedef time_t (*get_time_func)(t_fileData *);
#endif