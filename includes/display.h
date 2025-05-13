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



void display_sorted_files(bool an_error,t_dyn *files, t_flags *flags, bool is_directory, t_dynamic_format *dyn_format);

#endif