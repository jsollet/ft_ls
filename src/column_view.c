#include "../libft/includes/libft.h"
#include "../includes/types.h"
#include "../includes/column_view.h"
#include "../libft/includes/libftprintf.h"
#include "../includes/ft_printf1.h"

// les parametres a voir sous linux t->col_width surtout ;

void    init_terminal(t_term *t){
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &t->w);
	t->term_width = t->w.ws_col;
	t->col_width = 0;
	t->count = 0;
	t->max_len = 0;
}

void    update_terminal(t_term *t, char **files, size_t file_count) {
	t->count = file_count;
	for (size_t i = 0; i < file_count; i++) {
		size_t len = ft_strlen(files[i]);
		if (len > t->max_len)
			t->max_len = len;
	}
	t->col_width = t->max_len + 5;  // Ajout d'un espace pour la lisibilité ici avant 2, maintenant ok 
}

void    print_files_in_columns(char **files, t_term *t, char **color) {
	const char *reset = RESET_COLOR;
	int cols = t->term_width / t->col_width;
	if (cols == 0) cols = 1;

	int rows = (t->count + cols - 1) / cols;
	
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			size_t index = col * rows + row;
			if (index < t->count) {
				
				ft_printf1("%s", color[index]);
				ssize_t _unused = write(1, files[index], ft_strlen(files[index]));
				(void)_unused;
				ft_printf1("%s", reset);
				
				int space_count = t->col_width - ft_strlen(files[index]);
				for (int j = 0; j < space_count; j++) {
					ssize_t _unused=write(1, " ", 1);
					(void)_unused;
				}
			}
		}

		ssize_t _unused=write(1, "\n", 1);
		(void)_unused;
	}
}

void    list_directory_column(char *arr[], char *color[], int arr_length){
	t_term t;
	init_terminal(&t);
	update_terminal(&t, arr, arr_length);
	print_files_in_columns(arr, &t, color);
}

void	list_directory_line(char *arr[], char *color[],int arr_length){
	const char *reset = RESET_COLOR;
	int	i = 0;
	while (i < arr_length){
		ft_printf1( "%s%s%s\n", color[i], arr[i], reset);
		i++;
	}
}
