#ifndef HASHTABLE_H
# define HASHTABLE_H
# include <sys/stat.h>
#include <stdbool.h>
#include <stddef.h>

/* #define INODE_HASH_SIZE 2097143

typedef struct s_inodeSet {
	ino_t table[INODE_HASH_SIZE];
	int   count;
} t_inodeSet;

void inode_set_init(t_inodeSet *set);
bool inode_set_contains(t_inodeSet *set, ino_t ino);
void inode_set_add(t_inodeSet *set, ino_t ino); */
# define INODE_HASH_SIZE 666

typedef struct s_inodeNode {
    ino_t inode;
    struct s_inodeNode *next;
} t_inodeNode;

typedef struct s_inodeSet {
    t_inodeNode *table[INODE_HASH_SIZE];
} t_inodeSet;

void inode_set_init(t_inodeSet *set);
bool inode_set_contains(t_inodeSet *set, ino_t ino);
void inode_set_add(t_inodeSet *set, ino_t ino);
void inode_set_free(t_inodeSet *set);
void debug_print_inode_stats(void);
#endif