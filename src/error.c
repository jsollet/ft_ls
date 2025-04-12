#include "../libft/includes/libft.h"
#include "../includes/error.h"

void set_exit_status(t_exit_status *exit_status, int code, const char *message) {
    if (code > exit_status->code) {
        exit_status->code = code;
        if (message != NULL){
            free(exit_status->message);
            exit_status->message = ft_strdup(message);
        }
    }
}