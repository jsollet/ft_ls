#include "../libft/includes/libft.h"
#include "../includes/types.h"
#include "../includes/ministack.h"
#include "../libft/includes/libftprintf.h"

// global
//static t_stack *stack_freelist = NULL;  // Liste de noeuds à réutiliser


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
	//ft_printf("-----Empilé %s\n", path );
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

t_stack *pop_node(t_stack **stack) {
	if (!*stack) return NULL;
	t_stack *node = *stack;
	*stack = node->next;
	node->next = NULL;
	return node;
}


void push_existing_node(t_stack **stack, t_stack *node) {
	node->next = *stack;
	*stack = node;
}

void reverse_stack(t_stack **stack) {
	t_stack *reversed = NULL;
	while (*stack) {
		t_stack *node = pop_node(stack);
		push_existing_node(&reversed, node);
	}
	*stack = reversed;
}

void reverse_stack_old(t_stack **stack) {
	t_stack *reversed = NULL;
	while (*stack) {
		push(&reversed, pop(stack));
	}
	*stack = reversed;
}

void print_stack(t_stack *stack) {
	ft_printf("Path: ");
	while (stack != NULL) {
		ft_printf("%s (%zu)->", stack->path, stack->count);
		stack = stack->next;
	}
	ft_printf("\n");
}
/*
t_stack *get_stack_node(void) {
	t_stack *node;

	if (stack_freelist) {
		node = stack_freelist;
		stack_freelist = stack_freelist->next;
	} else {
		node = malloc(sizeof(t_stack));
		if (!node)
			return NULL;
	}
	return node;
}

void recycle_stack_node(t_stack *node) {
	node->next = stack_freelist;
	stack_freelist = node;
}

void push(t_stack **stack, const char *path) {
	t_stack *new_node = get_stack_node();
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
	recycle_stack_node(top);
	return path;
}

void free_stack_freelist(void) {
	t_stack *tmp;
	while (stack_freelist) {
		tmp = stack_freelist;
		stack_freelist = stack_freelist->next;
		free(tmp);
	}
} */