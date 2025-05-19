#include "../libft/includes/libft.h"
#include "../includes/hashtable.h"


static size_t inode_set_hits = 0;
static size_t inode_set_misses = 0;

static unsigned long hash_inode(ino_t ino) {
	return ino % INODE_HASH_SIZE;
}

void inode_set_init(t_inodeSet *set) {
	memset(set->table, 0, sizeof(set->table));
}

bool inode_set_contains(t_inodeSet *set, ino_t ino) {
	unsigned long idx = hash_inode(ino);
	t_inodeNode *current = set->table[idx];

	while (current) {
		if (current->inode == ino){
			inode_set_hits++;
			return true;
		}
		current = current->next;
	}
	inode_set_misses++;
	return false;
}

void inode_set_add(t_inodeSet *set, ino_t ino) {
	if (inode_set_contains(set, ino))
		return; // déjà présent

	unsigned long idx = hash_inode(ino);
	t_inodeNode *new_node = malloc(sizeof(t_inodeNode));
	if (!new_node)
		return;
	new_node->inode = ino;
	new_node->next = set->table[idx];
	set->table[idx] = new_node;
}

void inode_set_free(t_inodeSet *set) {
	for (int i = 0; i < INODE_HASH_SIZE; i++) {
		t_inodeNode *current = set->table[i];
		while (current) {
			t_inodeNode *next = current->next;
			free(current);
			current = next;
		}
		set->table[i] = NULL;
	}
}