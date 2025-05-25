#ifndef TYPES_H
# define TYPES_H
# include <stddef.h>
# include <stdbool.h>
# include <time.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/ioctl.h>
# include <limits.h>
# include "./color.h"

#define SIX_MONTHS_IN_SECONDS	15552000
#define MAX_PATH_LEN			1024
#define FILE_NAME_MAX     		NAME_MAX 
#define OWNER_NAME_MAX    		64
#define GROUP_NAME_MAX    		64



#define DISPLAY_FORMAT_LONG_G "%-10s%c %3ld %-*s %*lld %-10s %-10s"
#define DISPLAY_FORMAT_LONG_G_COLOR "%-10s%c %3ld %-*s %*lld %-10s %s%-10s%s"

#define DISPLAY_FORMAT_LONG "%-10s%c %3ld %-*s %-*s %*lld %-10s %-10s"
#define DISPLAY_FORMAT_LONG_COLOR "%-10s%c %3ld %-*s %-*s %*lld %-10s %s%-10s%s"

#define DISPLAY_FORMAT_INACCESSIBLE "%-10s%c %3s %-*s %-*s %*s %-10s %-10s\n"
#define DISPLAY_FORMAT_INACCESSIBLE_G "%-10s%c %3s %-*s %*s %-10s %-10s\n"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

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
	bool    e;
	bool    at; 
	bool    u;
	bool    U;
	bool    f;
	bool    g;
	bool    d;
	bool    one;
	bool    color;
	bool	acl;
	bool	attr;
	bool	extended;

}   t_flags;

typedef struct s_xttr
{
	char *name;
	ssize_t size;
	char *value;
} t_attr;

typedef struct s_special_bit {
	mode_t bit;
	mode_t exec;
	char   set;
	char   no_exec;
	int    pos;
}	t_special_bit;

typedef struct s_dynamic_format {
	size_t		max_name_width;
	size_t		max_owner_width;
	size_t		max_group_width;
	size_t		max_size_width;
}	t_dynamic_format;

typedef struct s_fileOwner {
	char            owner[OWNER_NAME_MAX + 1];
	char            group[GROUP_NAME_MAX + 1];
} t_fileOwner;

typedef struct s_fileXattr {
	t_attr          *xattrs;
	char*           acl_text;
	int             xattr_count;
	bool            print_xattrs;
	char            has_acl;
	char            has_xattr;
} t_fileXattr;

typedef struct s_fileMeta {
	ino_t           st_ino;
	char            fileType;
	char            permission[11];
	long            linkNumber;
	long long       fileSize;
	unsigned long	st_mtime_nsec;
	time_t          st_mtimes;
	unsigned long	st_atime_nsec;
	time_t          st_atimes;
	char            lastModified[20];
	long long       blocSize;
} t_fileMeta;

typedef struct s_fileData
{
	bool            valid;        
	char*           fileName;
	unsigned char	d_type;
	char*           absolutePath;
	char            link_target_buf[PATH_MAX];
	bool            argument;

	
	t_fileOwner     ownership;
	t_fileMeta      meta;
	t_fileXattr     xattr;

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
