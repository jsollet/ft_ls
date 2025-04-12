#ifndef ERROR_H
# define ERROR_H

#include <stdlib.h>
#include "types.h"
#include "../libft/includes/libft.h"
#include "../libft/includes/libftprintf_fd.h"

typedef struct s_exit_status {
    int code;           // Code d'erreur (ex: 0 pour succès, 1 pour erreur, etc.)
    char *message;      // Message d'erreur (si besoin)
} t_exit_status;

void set_exit_status(t_exit_status *exit_status, int code, const char *message);

#endif