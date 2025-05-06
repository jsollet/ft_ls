#ifndef MINISTACK_H
# define MINISTACK_H
#include <stdlib.h>
#include <errno.h>
#include "types.h"
#include "../libft/includes/libft.h"

void push(t_stack **stack, const char *path);
char *pop(t_stack **stack);
void print_stack(t_stack *stack);
void reverse_stack(t_stack **stack);
// test pool ??
/* t_stack *get_stack_node(void);
void recycle_stack_node(t_stack *node);
void free_stack_freelist(void); */
#endif
