#include "../includes/color.h"
#include "../includes/color_parser.h"


bool    is_executable(t_fileData *file) {
    return file->permission[3] == 'x' || file->permission[6] == 'x' || file->permission[9] == 'x';
}

bool    is_sticky(t_fileData *file){
    return file->permission[0] == 'd' && (file->permission[9] == 't' || file->permission[9] == 'T');
}

bool    is_setuid(t_fileData *file){
    return file->permission[3] == 's' || file->permission[3] == 'S';
}

bool    is_setgid(t_fileData *file){
    return file->permission[6] == 's' || file->permission[6] == 'S';
}

bool is_socket(t_fileData *file){
    return file->permission[0] == 's';
}

bool is_fifo(t_fileData *file){
    return file->permission[0] == 'p';
}

bool is_block_device(t_fileData *file){
    return file->permission[0] == 'b';
}

bool is_char_device(t_fileData *file){
    return file->permission[0] == 'c';
}

void	display_file_in_color(t_fileData *file, t_flags *flags, char *fileName){
	const char *color = RESET_COLOR;
	if (flags->color)
	{
        if (file->fileType == 'd') {
            color = BLUE_COLOR;  // Dossiers en bleu
        } else if (file->fileType == 'x') {
            color = GREEN_COLOR; // Fichiers exécutables en vert
        } else if (file->fileType == 'l') {
            color = CYAN_COLOR; // Liens symboliques en jaune
        }
    }
	ft_printf("%s%s%s", color, fileName, RESET_COLOR);
}

// a ameliorer...


char	*select_color_new(t_fileData *file, t_color_rule *color_rules){
	for (int i = 0; color_rules[i].type != 0; i++) {
        if (file->fileType == color_rules[i].type) {
            if (!color_rules[i].condition || color_rules[i].condition(file)) {
                return color_rules[i].color;
            }
        }
    }
    return RESET_COLOR;
}