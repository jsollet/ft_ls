#include "../libft/includes/libft.h"
#include "../includes/types.h"
#include "../includes/sort.h"
#include "../includes/dynamic_array_string.h"
#include "../includes/attr_acl.h"
#include "../includes/ministack.h"
#include "../includes/error.h"
#include "../includes/argument.h"
#include "../includes/color.h"
#include "../includes/color_parser.h"
#include "../includes/column_view.h"
#include "../includes/utils.h"
#include "../includes/getter.h"
#include "../includes/list_directory_helper.h"
#include "../includes/display.h"



#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

#define DEBUG false




bool    parse_flags(int argc, char *argv[], t_flags *flags){
	int index = 1;
	while (index < argc){
		if (argv[index][0] == '-' && argv[index][1] == '-'){
			if (ft_strcmp(argv[index], "--color") == 0) {
				flags->color = true;
			} else if (ft_strcmp(argv[index], "--acl") == 0){
				flags->acl = true;
				flags->e = true;
			} else if (ft_strcmp(argv[index], "--attr") == 0){
				flags->attr = true;
				flags->at = true;
			} else if (ft_strcmp(argv[index], "--extended") == 0){
				flags->extended = true;
			} else {
				ft_printf_fd(2,"ft_ls: invalid option -- '%s'\n", argv[index]);
				ft_printf_fd(2, "usage: ./ft_ls [-@RUalrtufgdeU1] [--color --acl --attr --extended][file ...]\n");
				return false; //voir pour les erreur et leurs gestion
			}
		} else
		if (argv[index][0] == '-' && argv[index][1] != '\0'){
			int position = 1;
			while (argv[index][position] != '\0'){
				if (argv[index][position] == 'R') flags->bigR = true;
				else if (argv[index][position] == 'r') flags->r = true;
				else if (argv[index][position] == 'a') flags->a = true;
				else if (argv[index][position] == 'l') flags->l = true;
				else if (argv[index][position] == 't') flags->t = true;
				else if (argv[index][position] == 'g') {flags->g = true; flags->l = true;}
				else if (argv[index][position] == 'd') flags->d = true;
				else if (argv[index][position] == 'u') flags->u = true;  //bonus acl
				else if (argv[index][position] == 'e') flags->e = true;  //bonus acl
				else if (argv[index][position] == '@') flags->at = true;  //bonus acl
				else if (argv[index][position] == 'U') flags->U = true;
				else if (argv[index][position] == 'f') {flags->f = true; flags->U = true; flags->a = true;}
				else if (argv[index][position] == '1') flags->one = true;
				else {
					ft_printf_fd(2,"ft_ls: invalid option -- '%c'\n", argv[index][position]);
					ft_printf_fd(2, "usage: ./ft_ls [-@RUalrtufgdeU1] [--color --acl --attr --extended][file ...]\n");
					return false; //voir pour les erreur et leurs gestion
				}
				position++;
			}
		}
		else if (argv[index][0] == '-' && argv[index][1] == '\0'){
			ft_printf_fd(2, "ft_ls: invalid option -- '-'\n");
			ft_printf_fd(2, "usage: ./ft_ls [-@RUalrtufgdeU1] [--color --acl --attr --extended][file ...]\n");
			return false;
		}
		index++;
	}
	return true;
}





int main(int argc, char *argv[]) {
	// Forcer la locale "C" pour éviter toute gestion locale spéciale
    setenv("LC_ALL", "C", 1);
	bool has_valid_input = false;
	bool path_was_given = false;

	int		double_dash_position = find_double_dash(argc, argv);
	t_flags flags = {0};
	t_exit_status exit_status = {};
	if (!parse_flags(double_dash_position, argv, &flags)){
		set_exit_status(&exit_status, 2, NULL);
		return exit_status.code;
	}

	t_stack *directories = NULL;
	t_stack *files_argument = NULL;
	t_stack *raw_argument = NULL; // pour flag -d 
	
	

	for (int i = 1; i < double_dash_position; i++){
		if (argv[i][0] == '-'){continue;}

		path_was_given = true;
		if (flags.d){
			push(&raw_argument, argv[i]),
			has_valid_input = true;
		}
		else {
			if (process_path(&directories, &files_argument, argv[i], &exit_status)) {
				has_valid_input = true;
			}
		}	
	}

	for (int i = double_dash_position + 1; i < argc; i++){
		if (flags.d){
			push(&raw_argument, argv[i]),
			has_valid_input = true;
		} else {
			if (process_path(&directories, &files_argument, argv[i], &exit_status)) {
				has_valid_input = true;
			}
		}
	}
	
	// Aucun chemin fourni → on ajoute "." par défaut
	if (!path_was_given) {
		process_path(&directories, &files_argument, ".", &exit_status);
		has_valid_input = true;
	}

	if (!has_valid_input) {// ajout
		return exit_status.code;
	}
	if (flags.d){
		list_directory(&flags, &directories, &raw_argument, &exit_status);
	} else {
		list_directory(&flags, &directories, &files_argument, &exit_status);
	}
	
	free(exit_status.message);
	return exit_status.code;
}