#ifndef COLOR_PARSER_H
# define COLOR_PARSER_H
#include "./color.h"
#include "./types.h"
#include "../libft/includes/libft.h"
#include "../libft/includes/libftprintf.h"

typedef struct s_color_rule {
	char    type;
	bool    (*condition)(t_fileData *);
	char    *color; 
} t_color_rule;

bool    is_executable(t_fileData *file);
bool    is_sticky(t_fileData *file);
bool    is_setuid(t_fileData *file);
bool    is_setgid(t_fileData *file);
bool    is_socket(t_fileData *file);
bool    is_fifo(t_fileData *file);
bool    is_block_device(t_fileData *file);
bool    is_char_device(t_fileData *file);
char	*select_color_new(t_fileData *file, t_color_rule *color_rules);
#endif