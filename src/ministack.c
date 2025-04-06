#include "../libft/includes/libft.h"
#include "../includes/types.h"
#include "../includes/ministack.h"
#include "../libft/includes/libftprintf.h"

void push(t_stack **stack, const char *path) {
	t_stack *new_node = malloc(sizeof(t_stack));
	if (!new_node) {
		perror("malloc");
		return;
	}
	new_node->path = ft_strdup(path);
	new_node->next = *stack;
	if (*stack)
		new_node->count = (*stack)->count + 1;  
	else
		new_node->count = 1;  
	*stack = new_node;
}

char *pop(t_stack **stack) {
	if (*stack == NULL) return NULL;
	t_stack *top = *stack;
	*stack = top->next;
	char *path = top->path;
	if (*stack)
		(*stack)->count = top->count - 1;
	free(top);
	return path;
}

void print_stack(t_stack *stack) {
	ft_printf("Path: ");
	while (stack != NULL) {
		ft_printf("%s (%zu)->", stack->path, stack->count);
		stack = stack->next;
	}
	ft_printf("\n");
}