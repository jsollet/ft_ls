#ifndef MINISTACK_H
# define MINISTACK_H
#include <stdlib.h>
#include <errno.h>
#include "types.h"
#include "../libft/includes/libft.h"

void push(t_stack **stack, const char *path);
char *pop(t_stack **stack);
void print_stack(t_stack *stack);
#endif
