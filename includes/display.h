#ifndef DISPLAY_H
# define DISPLAY_H

#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include "./types.h"
#include "./dynamic_array_string.h"
#include "./sort.h"
#include "./color.h"
#include "./color_parser.h"
#include "./column_view.h"
#include "./utils.h"

void	display_sorted_files(bool an_error,t_dyn *files, bool is_directory,t_context *ctx);

#endif