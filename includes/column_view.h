#ifndef COLUMN_VIEW_H
# define COLUMN_VIEW_H

#include "./types.h"
#include "../includes/color.h"
#include "../includes/color_parser.h"
#include "../libft/includes/libft.h"
#include "../libft/includes/libftprintf.h"
#include "../libft/includes/libftprintf_fd.h"

void    init_terminal(t_term *t);
void    update_terminal(t_term *t, char **files, size_t file_count);
void    print_files_in_columns(char **files, t_term *t, char **color);
void    list_directory_column(char *arr[], char *color[], int arr_length);
void	list_directory_line(char *arr[], char *color[],int arr_length);

#endif